/*
 *  shdnet.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/12/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */
#include "common.h"
#include "shdurl.h"
#include "shd_request.h"
#include "shd_thread.h"
#include <boost/lexical_cast.hpp>

class SHDThreadPrivate
{
public:
	SHDThreadPrivate( SHDThread *parent, boost::asio::io_service &io_service );
	void call( const SHDRequest &req );
	
	std::ostream *output;
	SHDRequest reqsource;
	bool in_progress;
	void move_parent( SHDThread *parent );
	
private:
	SHDThread * q_ptr;
	
	boost::asio::ip::tcp::resolver _resolver;
	boost::asio::ip::tcp::socket   _sock;
	boost::asio::streambuf _req;
	boost::asio::streambuf _resp;
	boost::asio::streambuf _cont;
	
	size_t _fsize;
	size_t _consumed;
	bool   _chunked;
	bool   _last_chunk_finished;
	size_t _last_chunk_size;
	
	void handle_resolve( const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint_iter );
	void handle_connect( const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint_iter );
	void handle_write_request( const boost::system::error_code &e );
	void handle_read_status_line( const boost::system::error_code &e );
	void handle_read_headers( const boost::system::error_code &e);
	void handle_read_content(const boost::system::error_code& err);

	void internal_reset();
};

SHDThread::SHDThread( boost::asio::io_service &io_service )
{
	d_ptr = boost::shared_ptr<SHDThreadPrivate>(new SHDThreadPrivate( this, io_service ));
}

SHDThread::SHDThread ( const SHDThread &other ) : d_ptr(other.d_ptr)
{
	d_ptr->move_parent( this );
}

SHDThread& SHDThread::operator=( const SHDThread &other )
{
	d_ptr = other.d_ptr;
	d_ptr->move_parent( this );
	return *this;
}

void SHDThread::call( const SHDRequest &req, std::ostream *outp)
{
	d_ptr->output = outp;
	d_ptr->call(req);
	d_ptr->in_progress = true;
}

bool SHDThread::is_in_progress() const
{
	return d_ptr->in_progress;
}

SHDRequest SHDThread::request() const
{
	return d_ptr->reqsource;
}		

SHDThreadPrivate::SHDThreadPrivate( SHDThread *parent, boost::asio::io_service &io_service )
:output(0),  in_progress(false), q_ptr( parent ), _resolver(io_service), _sock(io_service), _fsize(-1), _consumed(0),
_chunked(false), _last_chunk_finished(false), _last_chunk_size(0)
{
	
}

void SHDThreadPrivate::internal_reset()
{
	in_progress = false;
	output = 0;
	_fsize = -1;
	_consumed = 0;
	_chunked = false;
	_sock.close();
}

void SHDThreadPrivate::move_parent( SHDThread *parent )
{
	q_ptr = parent;
}
void SHDThreadPrivate::call( const SHDRequest &req )
{
	SHDRequest nreq(req);
	nreq.set_raw_header("Connection","close");
	std::ostream request_stream( &_req );
	request_stream << nreq;
	
	reqsource = req;
	
	boost::asio::ip::tcp::resolver::query query(reqsource.url().host(), reqsource.url().protocol());
    _resolver.async_resolve(query,
							boost::bind(&SHDThreadPrivate::handle_resolve, this,
										boost::asio::placeholders::error,
										boost::asio::placeholders::iterator));
}

void SHDThreadPrivate::handle_resolve( const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint_iter )
{
	if ( !e )
	{
		boost::asio::ip::tcp::endpoint endp = *endpoint_iter;
		_sock.async_connect(endp, boost::bind(&SHDThreadPrivate::handle_connect, this,
											  boost::asio::placeholders::error, ++endpoint_iter));
	}
}

void SHDThreadPrivate::handle_connect( const boost::system::error_code &e, boost::asio::ip::tcp::resolver::iterator endpoint_iter )
{
	if ( !e )
	{
		boost::asio::async_write( _sock, _req, boost::bind( &SHDThreadPrivate::handle_write_request, 
														   this, boost::asio::placeholders::error ));
	}
	else if ( endpoint_iter != boost::asio::ip::tcp::resolver::iterator())
	{
		_sock.close();
		boost::asio::ip::tcp::endpoint endp = *endpoint_iter;
		_sock.async_connect(endp, boost::bind(&SHDThreadPrivate::handle_connect, this,
											  boost::asio::placeholders::error, ++endpoint_iter));
	}
	else
	{
		std::cerr << "Error: " << e.message() << "\n";
	}
}

void SHDThreadPrivate::handle_write_request( const boost::system::error_code &e )
{
	if ( !e )
	{
		boost::asio::async_read_until(_sock, _resp, "\r\n",
									  boost::bind( &SHDThreadPrivate::handle_read_status_line, this, boost::asio::placeholders::error ));
	}
	else
	{
		std::cerr << "Error: " << e.message() << "\n";
		
	}
}

void SHDThreadPrivate::handle_read_status_line( const boost::system::error_code &e )
{
	if (!e)
    {
		// Check that response is OK.
		std::istream response_stream(&_resp);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		DLOG(INFO) << "HTTP VERSION : " << http_version << std::endl
		<< "Status Code :" << status_code << std::endl
		<< "Status Message :" << status_message << std::endl;
		
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			LOG(ERROR) << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			LOG(ERROR) << "Response returned with status code ";
			LOG(ERROR) << status_code << "\n";
			return;
		}
		
		boost::asio::async_read_until(_sock, _resp, "\r\n\r\n",
									  boost::bind(&SHDThreadPrivate::handle_read_headers, this,
												  boost::asio::placeholders::error));
		
    }
    else
    {
		LOG(ERROR) << "Error: " << e << "\n";
    }
	
}

void SHDThreadPrivate::handle_read_headers(const boost::system::error_code& err)
{
	if (!err)
	{
		// Process the response headers.
		std::istream response_stream(&_resp);
		std::string header;
		DLOG(INFO) << "-----------" << std::endl;
		while (std::getline(response_stream, header) && header != "\r")
		{
			DLOG(INFO) << header << "\n";
			
			const size_t hpos = header.find_first_of(':');
			std::pair<std::string,std::string> hdrpair;
			if ( hpos != std::string::npos )
			{
				hdrpair.first = header.substr(0, hpos);
				hdrpair.second = header.substr(hpos+1);
				boost::trim(hdrpair.second);
			}
			
			switch ( tolower(header[0]) )
			{
			case 'c':
				if (boost::iequals(hdrpair.first, "content-length"))
				{
					try
					{
						_fsize = boost::lexical_cast<size_t>(hdrpair.second);
					}
					catch (boost::bad_lexical_cast &)
					{
						_fsize = -1;
					}
				}
				break;
			case 't':
				if (boost::iequals(hdrpair.first, "transfer-encoding"))
				{
					if (boost::iequals(hdrpair.second,"chunked"))
					{
						_chunked = false;
					}
				}
			}
						
			
		}
		
		DLOG(INFO) << "\n";
		
		if (output != 0 )
		{
			const size_t csize = _resp.size();
			if ( csize > 0 )
			{
				char *buf = new char[ csize + 1 ];
				std::istream contstream( &_resp );
				contstream.readsome(buf, csize);
				if ( !_chunked )
				{
					output->write(buf, csize);
				}
				else
				{
					// do chunked transfer
				}
				_consumed = csize;
				delete [] buf;
			}
		
			boost::asio::async_read(_sock, _cont,
								boost::asio::transfer_at_least(1),
								boost::bind(&SHDThreadPrivate::handle_read_content, this,
								boost::asio::placeholders::error));
		}
		
			
	} 
	else
	{
			LOG(ERROR) << "Error: " << err << "\n";
	}
}

void SHDThreadPrivate::handle_read_content(const boost::system::error_code& err)
{
	static char buf[ 1024 ];
    if (!err)
    {
		const size_t csize = _cont.size();
		if ( csize > 0 )
		{
			std::istream contstream( &_cont );
			const size_t write_size = (csize >= 1024 ? 1024 : csize);
			
			contstream.readsome(buf, 1024);
			if ( !_chunked )
			{
				output->write(buf, write_size);
			}
			else
			{
				// do chunked transfer
			}
			_consumed += write_size;
			// DLOG(INFO) << reqsource.url() << " -- writing (" << _consumed << " of " << _fsize << " bytes) \n";
			if (_fsize != -1)
			{
				q_ptr->progress( _consumed, _fsize );
			}
		}
		// Continue reading remaining data until EOF.
		boost::asio::async_read(_sock, _cont,
								boost::asio::transfer_at_least(1),
								boost::bind(&SHDThreadPrivate::handle_read_content, this,
											boost::asio::placeholders::error));
    }
	else if( err == boost::asio::error::eof )
	{
		DLOG(INFO) << reqsource.url() << " -- done\n";
		q_ptr->done();
		internal_reset();
	}
    else if (err != boost::asio::error::eof)
    {
		LOG(ERROR) << "Error: " << err << "\n";
    }
}


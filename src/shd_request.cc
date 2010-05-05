/*
 *  shd_request.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/7/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#include "common.h"
#include "shdurl.h"
#include "shd_request.h"
#include "shd_header.h"

class SHDRequestPrivate : public SHDHeaderPrivate
{
public:
	inline SHDRequestPrivate() : 
		SHDHeaderPrivate(),
		priority(SHDRequest::NormalPriority)
	{
		
	}
	
	~SHDRequestPrivate()
	{
		
	}
	
	SHDRequestPrivate( const SHDRequestPrivate &other ) 
	{
		url = other.url;
		priority = other.priority;
	}
	
	inline bool operator == ( const SHDRequestPrivate &other ) const
	{
		return ( url == other.url ) &&
		( priority == other.priority );
	}
	SHDUrl url;
	SHDRequest::Priority priority;
};


SHDRequest::SHDRequest( const SHDUrl &url )
: d_ptr( boost::shared_ptr<SHDRequestPrivate>( new SHDRequestPrivate ) )
{
	set_url(url);
}

SHDRequest::SHDRequest( const SHDRequest &other )
: d_ptr( other.d_ptr )
{
	
}

SHDRequest::~SHDRequest()
{
}

bool SHDRequest::operator==( const SHDRequest &other ) const
{
	return d_ptr == other.d_ptr || *d_ptr == *(other.d_ptr);
}

SHDRequest &SHDRequest::operator=( const SHDRequest &other )
{
	d_ptr = other.d_ptr;
	return *this;
}

SHDUrl &SHDRequest::url() const
{
	return d_ptr->url;
}

void SHDRequest::set_url( const SHDUrl &url )
{
	d_ptr->url = url;
	d_ptr->set_raw_header("Host", d_ptr->url.host());
}

SHDRequest::Priority SHDRequest::priority() const
{
	return d_ptr->priority;
}

void SHDRequest::set_priority( const SHDRequest::Priority p )
{
	d_ptr->priority = p;
}

std::string SHDRequest::header( const KnownHeaders header ) const
{
	return d_ptr->cooked_headers [header];
}

void SHDRequest::set_header( const KnownHeaders header, const std::string &value )
{
	d_ptr->set_cooked_header(header, value);
}

bool SHDRequest::has_raw_headers( const std::string &name )
{
	return d_ptr->find_raw_header(name)  != d_ptr->raw_headers.end();
}

std::string SHDRequest::raw_header( const std::string &name ) const
{
	SHDHeaderPrivate::raw_header_list_t::const_iterator it = d_ptr->find_raw_header(name);
	if ( it != d_ptr->raw_headers.end() )
	{
		return it->second;
	}
	return std::string();
}

void SHDRequest::set_raw_header( const std::string &name, const std::string &val )
{
	d_ptr->set_raw_header(name,val);
}

std::list<std::string> SHDRequest::raw_header_list() const
{
	return d_ptr->raw_header_keys();
}


// Header Private

void SHDHeaderPrivate::set_raw_header_internal( const std::string &key, const std::string &value )
{
	raw_header_list_t::iterator it = raw_headers.begin();
	raw_header_list_t::const_iterator end = raw_headers.end();
	
	while ( it != end )
	{
		if ( boost::iequals( it->first, key ) )
		{
			raw_headers.erase(it);
		}
		else 
		{
			++it;
		}
	}
	
	if ( value.empty() )
	{
		return;
	}
	
	raw_header_pair_t pair;
	pair.first = key;
	pair.second = value;
	
	raw_headers.push_back(pair);
}

static inline std::string headerName( const SHDRequest::KnownHeaders header )
{
	switch ( header )
	{
		case SHDRequest::ContentTypeHeader:
			return "Content-Type";
		case SHDRequest::ContentLengthHeader:
			return "Content-Length";
		case SHDRequest::LocationHeader:
			return "Location";
		case SHDRequest::LastModifiedHeader:
			return "Last-Modified";
		case SHDRequest::CookieHeader:
			return "Cookie";
		case SHDRequest::SetCookieHeader:
			return "Set-Cookie";
	}
	
	return std::string();
}

static inline SHDRequest::KnownHeaders parse_header_name( const std::string &headerName )
{
	switch (tolower(headerName[0]))
	{
		case 'c':
			if ( boost::iequals( headerName, "content-type" ) )
			{
				return SHDRequest::ContentTypeHeader;
			}
			else if ( boost::iequals( headerName, "content-length" ) )
			{
				return SHDRequest::ContentLengthHeader;
			}
			else if ( boost::iequals( headerName, "cookie" ) )
			{
				return SHDRequest::CookieHeader;
			}
		
			break;
			
		case 'l':
			if ( boost::iequals( headerName, "location" ) )
			{
				return SHDRequest::LocationHeader;
			}
			else if ( boost::iequals( headerName, "last-modified" ) )
			{
				return SHDRequest::LastModifiedHeader;
			}
			
			break;
		
		case 's':
			if ( boost::iequals( headerName, "set-cookie" ) )
			{
				return SHDRequest::SetCookieHeader;
			}
			
			break;
	}
	
	return SHDRequest::KnownHeaders(-1);
}

void SHDHeaderPrivate::parse_and_set_header(const std::string &key, const std::string &value )
{
	SHDRequest::KnownHeaders parsed_key = parse_header_name( key );
	if ( parsed_key != SHDRequest::KnownHeaders(-1))
	{
		if ( value.empty())
		{
			cooked_headers.erase(parsed_key);
		}
		else
		{
			cooked_headers.insert( std::pair<SHDRequest::KnownHeaders, std::string>::pair(parsed_key, value) );
		}
	}
}

void SHDHeaderPrivate::set_raw_header( const std::string &key, const std::string &value )
{
	if ( key.empty() )
	{
		return;
	}
	
	set_raw_header_internal(key, value);
	parse_and_set_header(key, value);
}


void SHDHeaderPrivate::set_all_raw_headers( const raw_header_list_t &lst )
{
	cooked_headers.clear();
	raw_headers = lst;
	
	raw_header_list_t::const_iterator it = raw_headers.begin();
	raw_header_list_t::const_iterator end = raw_headers.end();
	
	for (; it != end; ++it)
	{
		parse_and_set_header(it->first, it->second);
	}
}

void SHDHeaderPrivate::set_cooked_header( const SHDRequest::KnownHeaders header, const std::string &value )
{
	std::string name = headerName(header);
	if ( name.empty() )
	{
		std::cerr << "Invalid Header (" << header << ") received";
		return;
	}
	
	if ( value.empty() )
	{
		set_raw_header_internal(name, std::string());
		cooked_headers.erase( header );
	}
	else 
	{
		std::string raw_value = value;
		if ( raw_value.empty() )
		{
			return;
		}
		
		set_raw_header_internal(name, raw_value);
		cooked_headers.insert( std::pair<SHDRequest::KnownHeaders, std::string>(header, value) );
	}
}

std::list<std::string> SHDHeaderPrivate::raw_header_keys() const
{
	std::list<std::string> ret;
	raw_header_list_t::const_iterator it = raw_headers.begin();
	raw_header_list_t::const_iterator end = raw_headers.end();
	
	for( ; it != end; ++it )
	{
		ret.push_back(it->first);
	}
	
	return ret;
}

SHDHeaderPrivate::raw_header_list_t::const_iterator SHDHeaderPrivate::find_raw_header(const std::string &keyt) const
{
	raw_header_list_t::const_iterator it = raw_headers.begin();
	raw_header_list_t::const_iterator end = raw_headers.end();
	
	for( ; it != end; ++it )
	{
		if ( boost::iequals(keyt, it->first) )
		{
			return it;
		}
	}
	
	return end;
}


std::ostream &operator << ( std::ostream &outp, const SHDRequest &req )
{
	std::list<std::string> lst = req.raw_header_list();
	std::list<std::string>::iterator it = lst.begin();
	const std::list<std::string>::const_iterator end = lst.end();
	
	outp << "GET " << req.url().path() << " HTTP/1.1" << "\r\n" ;
	
	
	for ( ; it != end; ++it )
	{
		const std::string key = *it;
		outp << (key) << ": " << req.raw_header(key) << "\r\n";
	}
	outp << "\r\n";
	
	return outp;
}
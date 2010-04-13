/*
 *  main.cc
 *  Boost Single HTTP Dispatcher
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MIJIX. All rights reserved.
 *
 */

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

int main( int argc, char **argv )
{
	try 
	{
		if ( 2 != argc )
		{
			std::cerr << "Usage: TestSHD <host>" << std::endl;
			return 1;
		}
		
		boost::asio::io_service io;
		tcp::resolver resolver(io);
		tcp::resolver::query query(argv[1], "daytime");
		
		tcp::resolver::iterator endpoint_iterotor = resolver.resolve(query);
		tcp::resolver::iterator end;
		
		tcp::socket socket(io);
		boost::system::error_code error = boost::asio::error::host_not_found;
		
		while( error && endpoint_iterotor != end )
		{
			socket.close();
			socket.connect(*endpoint_iterotor++, error);
		}
		
		if (error)
		{
			throw boost::system::system_error(error);
		}
		
		for(;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;
			
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			if ( error == boost::asio::error::eof )
			{
				break;
			}
			else if (error)
			{
				throw boost::system::system_error(error);
			}
			
			std::cout.write(buf.data(), len);
		}
	}
	catch ( std::exception &e ) 
	{
		std::cerr << e.what() << std::endl;
	}
}
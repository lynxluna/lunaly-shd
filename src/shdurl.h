/*
 *  shdrequest.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_URL_H
#define __SHD_URL_H

class SHDUrl
{
public:
	const std::string protocol() const { return _protocol; }
	const std::string host() const { return _host; }
	const std::string path() const { return _path; }
	const std::string qs() const { return _qs; }
	bool  is_valid() const;
	const std::string to_string() const;
	const std::multimap<std::string,std::string> qs_map() const;
	
	SHDUrl( const std::string &strUrl );
	SHDUrl( const SHDUrl &other );
	SHDUrl( void );
	
	SHDUrl &operator =( const SHDUrl &other );
	bool operator ==( const SHDUrl &other ) const;
	
private:
	std::string _protocol;
	std::string _host;
	std::string _path;
	std::string _qs;
	
	friend std::ostream &operator << ( std::ostream &s, const SHDUrl & );
};

#endif __SHD_URL_H
/*
 *  shd_header.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/7/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_HEADER_H
#define __SHD_HEADER_H

class SHDHeaderPrivate
{
public:
	typedef std::pair<std::string, std::string> raw_header_pair_t;
	typedef std::list<raw_header_pair_t> raw_header_list_t;
	typedef std::map<SHDRequest::KnownHeaders, std::string> cooked_headers_map_t;
	
	raw_header_list_t raw_headers;
	cooked_headers_map_t cooked_headers;
	
	raw_header_list_t::const_iterator find_raw_header(const std::string &key ) const;
	std::list<std::string> raw_header_keys() const;
	
	void set_raw_header(const std::string &key, const std::string &val );
	void set_all_raw_headers( const raw_header_list_t &lst );
	void set_cooked_header( SHDRequest::KnownHeaders, const std::string &val );
	
private:
	void set_raw_header_internal( const std::string &key, const std::string &value );
	void parse_and_set_header( const std::string &key, const std::string &value );
};

#endif //__SHD_HEADER_H
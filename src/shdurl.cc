/*
 *  shdrequest.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#include "common.h"
#include "shdurl.h"
#include <sstream>

static const std::string _urlRe = "(https|http|ftp)\\:\\/\\/(([\\w|\\-]+\\.)*([\\w|\\-]*))((\\/[\\w|\\-]+)*)(\\/.*|\\?.*)";

static inline bool _check_url_validity( const std::string &strUrl )
{
	static boost::regex re (_urlRe );
	boost::cmatch matches;
	
	return boost::regex_match(strUrl.c_str(), matches, re);
}

std::ostream &operator<<( std::ostream &outp, const SHDUrl &url )
{
	outp << url.to_string();
	return outp;
}

SHDUrl::SHDUrl( const std::string &strUrl )
{
	if (!_check_url_validity(strUrl))
	{
		boost::format formatter ("Error parsing URL: %1%");
		formatter % strUrl;
		throw std::runtime_error( formatter.str() );
	}
	else 
	{
		static boost::regex re( _urlRe );
		boost::cmatch matches;
		
		if (boost::regex_match(strUrl.c_str(), matches, re))
		{
			try
			{
				_protocol = matches[1];
			}
			catch ( std::exception & )
			{
				_protocol = "";
				return;
			}
			
			try
			{
				_host	  = matches[2];
			}
			catch ( std::exception & )
			{
				_host = "";
				return;
			}
			
			// I AM SICK OF USING REGEX FOR PARSING THE URLS AS I WISH SO I JUST PARSE IT WITH
			// MERE ALGORITHM
			
			// split the string from it's protocol
			const std::string no_proto = strUrl.substr(strUrl.find_first_of("://")+3);
			const std::string full_p = no_proto.substr(no_proto.find_first_of('/'));
			std::vector<std::string> path_and_qs;
			boost::split(path_and_qs, full_p, boost::is_any_of("?"));
			try
			{
				_path = path_and_qs[0];
			}
			catch (std::exception &) {
				_path = "/";
			}
			
			if ( path_and_qs.size() > 1 )
			{
				_qs   = path_and_qs[1];
			}
		}
	}

}

SHDUrl::SHDUrl(void) 
: _protocol(std::string())
, _host(std::string())
, _path(std::string())
, _qs(std::string())
{
	
}

bool SHDUrl::operator==( const SHDUrl &other ) const
{
	return (_protocol == other._protocol) &&
	       (_host == other._host) &&
		   (_path == other._path) &&
	       (_qs   == other._qs);
}

SHDUrl::SHDUrl( const SHDUrl &other )
{
	_protocol = other._protocol;
	_host     = other._host;
	_path     = other._path;
	_qs		  = other._qs;
}

SHDUrl &SHDUrl::operator=( const SHDUrl &other )
{
	_protocol = other._protocol;
	_host     = other._host;
	_path     = other._path;
	_qs       = other._qs;
	
	return *this;
}

bool SHDUrl::is_valid() const
{
	return _check_url_validity(to_string());
}

const std::string SHDUrl::to_string() const
{
	std::stringstream outp;
	outp << _protocol << "://" << _host;
	if (_path.length() > 0)
	{
		outp << _path;
	}
	else 
	{
		outp << "/";
	}

	if ( _qs.length() > 0 )
	{
		outp << "?" << _qs;
	}
	return outp.str();	
}

const std::multimap<std::string,std::string> SHDUrl::qs_map() const
{
	std::multimap<std::string,std::string> ret;
	const boost::char_separator<char> andsep("&");
	boost::tokenizer< boost::char_separator<char> > tokens( _qs, andsep );
	BOOST_FOREACH(std::string t, tokens)
	{
		std::vector<std::string> strs;
		boost::split(strs, t, boost::is_any_of("="));
		const std::pair<std::string,std::string> itempair(strs[0], strs[1]);
		ret.insert(itempair);
	}
	
	return ret;
}



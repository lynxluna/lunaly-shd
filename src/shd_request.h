/*
 *  shd_request.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/7/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_REQUEST_H
#define __SHD_REQUEST_H

class SHDRequestPrivate;
typedef boost::shared_ptr<SHDRequestPrivate> SHDRequestPrivatePtr;
class SHDRequest
{
public:
	
	enum KnownHeaders
	{
		ContentTypeHeader,
		ContentLengthHeader,
		LocationHeader,
		LastModifiedHeader,
		CookieHeader,
		SetCookieHeader,
	};
	
	enum Priority
	{
		RealtimePriority = 1,
		NormalPriority   = 3,
		LowPriority      = 5,
	};
	
	explicit SHDRequest( const SHDUrl &url = SHDUrl() );
	SHDRequest( const SHDRequest &req );
	~SHDRequest( void );
	bool operator ==( const SHDRequest &other ) const;
	SHDRequest &operator=( const SHDRequest &other );
	
	SHDUrl &url() const;
	void set_url( const SHDUrl &url );
	
	
	std::string header( const KnownHeaders ) const;
	void set_header( const KnownHeaders, const std::string &value );
	
	
	bool has_raw_headers( const std::string &name );
	
	std::string raw_header( const std::string & ) const;
	void set_raw_header( const std::string &name, const std::string &value );
	std::list<std::string> raw_header_list() const;
	
	
	Priority priority() const;
	void set_priority( const Priority p );
	
	
private:	
	SHDRequestPrivatePtr d_ptr;
	
	friend std::ostream &operator << ( std::ostream &, const SHDRequest & );
};

std::ostream &operator << ( std::ostream &outp, const SHDRequest &req );


#endif //__SHD_REQUEST_H

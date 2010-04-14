/*
 *  shd_auth.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/10/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef ___SHD_AUTH_INTERFACE
#define ___SHD_AUTH_INTERFACE
#include "shdurl.h"
#include "shd_request.h"
class SHDAuthInterface
{
public:
	virtual void set_token( const std::string & ) = 0 ;
	virtual std::string token() const = 0 ;
	virtual void set_token_secret( const std::string & ) = 0 ;
	virtual std::string token_secret() const = 0 ;
	
	virtual void sign_request( SHDRequest &req ) = 0;
};

#endif //___SHD_AUTH_INTERFACE
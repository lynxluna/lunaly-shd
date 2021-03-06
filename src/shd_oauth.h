/*
 *  shd_oauth.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/14/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_OAUTH_H
#define __SHD_OAUTH_H

#include "shd_auth.h"

class SHDOAuthPrivate;
class SHDOAuth : public SHDAuthInterface
{
public:
	virtual void set_token( const std::string & );
	virtual std::string token() const;
	virtual void set_token_secret( const std::string & );
	virtual std::string token_secret() const;
	
	virtual void sign_request( SHDRequest &req );
	
private:
	boost::shared_ptr<SHDOAuthPrivate> d_ptr;
};


#endif // __SHD_OAUTH_H
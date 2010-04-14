/*
 *  shd_auth_simple_realm
 *  BSHD
 *
 *  Created by Lynx Luna on 4/14/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#include "common.h"
#include "shdurl.h"
#include "shd_request.h"
#include "shd_auth_simple_realm.h"
#include "util.h"

class SHDAuthSimpleRealmPrivate
{
public:
	std::string uname;
	std::string password;
	
 	std::string base64_out() const;
};


std::string SHDAuthSimpleRealmPrivate::base64_out() const
{
	const std::string outp( uname + ":" + password );
	return base64encode(reinterpret_cast<const unsigned char*>(outp.c_str()), outp.length());
}


SHDAuthSimpleRealm::SHDAuthSimpleRealm()
{
	d_ptr = boost::shared_ptr<SHDAuthSimpleRealmPrivate>(new SHDAuthSimpleRealmPrivate);
}

void SHDAuthSimpleRealm::set_token( const std::string &t )
{
	d_ptr->uname = t;
}

std::string SHDAuthSimpleRealm::token() const
{
	return d_ptr->uname;
}

void SHDAuthSimpleRealm::set_token_secret( const std::string &t )
{
	d_ptr->password = t;
}

std::string SHDAuthSimpleRealm::token_secret() const
{
	return d_ptr->password;
}

void SHDAuthSimpleRealm::sign_request( SHDRequest &req )
{
	req.set_raw_header("Authorization", std::string("BASIC ") + d_ptr->base64_out());
}
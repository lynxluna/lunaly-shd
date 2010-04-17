/*
 *  util.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/14/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#include "common.h"
#include "util.h"

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <sstream>
std::string base64encode( const unsigned char *data, const size_t length )
{
	BIO *bmem, *b64;
	BUF_MEM *bptr;
	
	b64 = BIO_new( BIO_f_base64() );
	bmem = BIO_new( BIO_s_mem() );
	b64 = BIO_push( b64, bmem );
	BIO_write( b64, data, length );
	BIO_flush( b64 );
	BIO_get_mem_ptr(b64, &bptr);
	
	std::string ret( bptr->data, bptr->length-1);
	
	BIO_free_all(b64);
	
	return ret;
}

size_t hexstring_to_size( const std::string &hexstr )
{
	std::istringstream iss( hexstr );
	size_t ret;
	iss >> std::hex >> ret;
	return ret;
}
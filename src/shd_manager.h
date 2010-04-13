/*
 *  shd.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX. All rights reserved.
 *
 */

#ifndef		__SHD_H
#define		__SHD_H

class SHDManagerPrivate;
class SHDManager
{
public:
	SHDManager( const size_t max_threads = 4);
	void call( const SHDRequest &req, std::ostream *output=&std::cout, bool queued = true );
	void start();
	void reset();
private:
	boost::shared_ptr<SHDManagerPrivate> d_ptr;
};

#endif		// __SHD_H
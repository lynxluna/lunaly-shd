/*
 *  shdnet.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/12/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_THREAD_H
#define __SHD_THREAD_H

class SHDThreadPrivate;
class SHDThread
{
public:
	explicit SHDThread(boost::asio::io_service &io_service);
	SHDThread(const SHDThread &other );
	
	SHDThread &operator=( const SHDThread &other );
	void call( const SHDRequest &req, std::ostream *output = 0 );
	
	
	SHDRequest request() const;
	bool is_in_progress() const;
	
	boost::signal<void()> done;
	boost::signal<void(size_t current, size_t total)> progress;
private:
	boost::shared_ptr<SHDThreadPrivate> d_ptr;
};

#endif
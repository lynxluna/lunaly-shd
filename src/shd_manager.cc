/*
 *  shd.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX. All rights reserved.
 *
 */
#include "common.h"
#include <boost/noncopyable.hpp>
#include "shdurl.h"
#include "shd_request.h"
#include "shd_thread.h"
#include "shd_manager.h"

class SHDManagerPrivate
{
	
public:
	SHDManagerPrivate( const size_t max_queue = 4);	
	static boost::asio::io_service io_service;
	
	size_t maximum_queue;
	std::vector<SHDThread> queued_threads;
	std::vector<SHDThread> immediate_threads;
	std::vector< std::pair<SHDRequest, std::ostream *> > queued_req;
	
	void add_queued_call( const SHDRequest &req, 
						 std::ostream *outp = &std::cout );
	void add_immediate_call( const SHDRequest &req, std::ostream *outp = &std::cout );
	
	std::vector<SHDThread>::iterator find_spare_queued_thread();
	std::vector<SHDThread>::iterator find_spare_immediate_thread();
	
	void thread_done();
	
};

boost::asio::io_service SHDManagerPrivate::io_service;

SHDManager::SHDManager( const size_t max_threads ) : d_ptr ( new SHDManagerPrivate( max_threads) )
{
}

void SHDManager::call( const SHDRequest &req, std::ostream *outp, bool queued)
{
	if ( queued )
	{
		d_ptr->add_queued_call( req, outp );
	}
	else
	{
		d_ptr->add_immediate_call( req, outp );
	}

}

void SHDManager::start()
{
	d_ptr->io_service.run();
}

void SHDManager::reset()
{
	d_ptr->io_service.reset();
}

void SHDManagerPrivate::thread_done()
{
	std::vector<SHDThread>::iterator ith= find_spare_queued_thread();
	
	if (ith != queued_threads.end() && queued_req.size() > 0 )
	{
		const std::pair<SHDRequest, std::ostream*> &reqpair = queued_req.back();
		DLOG(INFO) << "A queued thread done doing its thingies"
		           << "It is time for " << reqpair.first.url() << " to rock";
		ith->call( reqpair.first, reqpair.second );
		queued_req.pop_back();
	}
}
															

SHDManagerPrivate::SHDManagerPrivate( const size_t max_queue ) : maximum_queue(max_queue)
{
	for ( size_t i = 0; i < maximum_queue; ++ i )
	{
		queued_threads.push_back(SHDThread(SHDManagerPrivate::io_service));
	}
	
	for ( size_t i = 0; i < maximum_queue; ++ i )
	{
		queued_threads[i].done.connect(boost::bind(&SHDManagerPrivate::thread_done, this));
	}
}

void SHDManagerPrivate::add_queued_call( const SHDRequest &req, std::ostream *outp )
{
	std::vector<SHDThread>::iterator ith= find_spare_queued_thread();
	if ( ith == queued_threads.end())
	{
		queued_req.push_back(std::pair<SHDRequest, std::ostream *>(req, outp));
	}
	else 
	{
		ith->call( req, outp );
	}

}

void SHDManagerPrivate::add_immediate_call( const SHDRequest &req, std::ostream *outp )
{
	std::vector<SHDThread>::iterator ith = find_spare_immediate_thread();
	if ( ith == immediate_threads.end())
	{
		immediate_threads.push_back(SHDThread(SHDManagerPrivate::io_service));
		immediate_threads.back().call( req, outp );
	}
	else
	{
		ith->call( req, outp );
	}

}

std::vector<SHDThread>::iterator SHDManagerPrivate::find_spare_queued_thread()
{
	std::vector<SHDThread>::iterator end = queued_threads.end();
	std::vector<SHDThread>::iterator it = queued_threads.begin();
	for (; it != end; ++it )
	{
		if ( !it->is_in_progress() )
		{
			return it;
		}
	}
	
	return end;
}

std::vector<SHDThread>::iterator SHDManagerPrivate::find_spare_immediate_thread()
{
	std::vector<SHDThread>::iterator end = immediate_threads.end();
	std::vector<SHDThread>::iterator it = immediate_threads.begin();
	for (; it != end; ++it )
	{
		if ( !it->is_in_progress() )
		{
			return it;
		}
	}
	
	return end;
}
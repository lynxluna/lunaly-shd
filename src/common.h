/*
 *  common.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_COMMON_H
#define __SHD_COMMON_H
#if defined(__OBJC__) && defined(__APPLE__)
#import <Cocoa/Cocoa.h>
#endif


#ifdef _MSC_VER
#define _WIN32_WINNT 0x500
#define NOMINMAX // prevent clash with std::min and std::max
#endif

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <exception>
#include <map>
#include <queue>
#include <list>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include "config.h"
#endif
#include <glog/logging.h>

#if defined(_MSC_VER)
#undef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/signal.hpp>
#endif



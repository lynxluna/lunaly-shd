/*
 *  common.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#if defined(__OBJC__) && defined(__APPLE__)
#import <Cocoa/Cocoa.h>
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
#include <ext/hash_map>
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
#include <glog/logging.h>
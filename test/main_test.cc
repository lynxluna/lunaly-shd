/*
 *  main_test.cc
 *  BSHD
 *
 *  Created by Lynx Luna on 4/5/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#include "common.h"
#include "shdurl.h"
#include "shd_request.h"
#include "shd_thread.h"
#include "shd_manager.h"
#include "shd_auth_simple_realm.h"
#include <gtest/gtest.h>


#include "SHDUrlTest.h"
#include "SHDRequestTest.h"
#include "SHDThreadTest.h"
#include "util.h"
#include "GlobalSHDTest.h"

#include <sstream>

int main(int argc, char **argv) 
{
	google::InitGoogleLogging(argv[0]);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
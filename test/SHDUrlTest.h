/*
 *  SHDUrlTest.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/7/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

#ifndef __SHD_URLTEST_H
#define __SHD_URLTEST_H

class SHDUrlTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		_valid_url = "http://www.example.com/abc/def?ghi=klm&opq=rst";
		_invalid_url = "http:xxxaeuaoueoauoeau";
		_other_valid_url  = "http://www.myquran.org/";
	}
	
	std::string _valid_url;
	std::string _invalid_url;
	std::string _other_valid_url;
	
};

TEST_F(SHDUrlTest, IsUrlValid)
{
	SHDUrl url(_valid_url);
	ASSERT_TRUE(url.is_valid()) << "The URL is expected to be valid";
	SHDUrl url2(_other_valid_url);
	ASSERT_TRUE(url2.is_valid()) << "The URL is expected to be valid";
	
}

TEST_F(SHDUrlTest, ParseInvalidUrl)
{
	try
	{
		SHDUrl url(_invalid_url);
		EXPECT_FALSE(true) << "This string shouldn't able to be parsed";
	}
	catch (std::runtime_error &)
	{
		EXPECT_TRUE(true);
	}
}

TEST_F(SHDUrlTest, ParseUrl)
{
	SHDUrl url(_valid_url);
	EXPECT_STREQ(url.protocol().c_str(), "http");
	EXPECT_STREQ(url.host().c_str(), "www.example.com");
	EXPECT_STREQ(url.path().c_str(), "/abc/def");
	EXPECT_STREQ(url.qs().c_str(), "ghi=klm&opq=rst");
}

TEST_F(SHDUrlTest, MapQueryString)
{
	SHDUrl url(_valid_url);
	std::multimap<std::string, std::string> qsmap = url.qs_map();
	for (std::multimap<std::string, std::string>::iterator it = qsmap.begin();
		 it != qsmap.end();
		 ++it)		
	{
		const std::pair<std::string,std::string> &item = (*it);
		if (item.first == "ghi")
		{
			EXPECT_STREQ(item.second.c_str(), "klm");
		}
		
		if (item.first == "opq")
		{
			EXPECT_STREQ(item.second.c_str(), "rst");
		}
	}
}

TEST_F(SHDUrlTest, EqualityTest)
{
	SHDUrl url1(_valid_url);
	SHDUrl url2(_valid_url);
	SHDUrl url3("http://www.myquran.org/");
	
	
	ASSERT_TRUE(url1 == url2) << " URL1 should be the same as URL2";
	ASSERT_FALSE(url1 == url3) << " URL1 should not be the same as URL3";
	ASSERT_FALSE(url2 == url3) << " URL2 should not be the same as URL3";
}

TEST_F(SHDUrlTest, StreamOutputTest)
{
	std::stringstream ss;
	SHDUrl url1(_valid_url);
	ss << url1;
	ASSERT_STREQ( ss.str().c_str(), _valid_url.c_str() );
	std::cout << "Current URL IS:" << url1 << std::endl;
}

#endif
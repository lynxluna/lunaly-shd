/*
 *  SHDRequestTest.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/7/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */


class SHDRequestTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		_source_url = SHDUrl("http://www.digicelebs.com/cpg/albums/SC/Emily_Scott/"
							 "emily_scott_photoshoot_uhq_nude_ultra_sexy_04.jpg?auth=true");
		
		_should_be_header_list.push_back("signature");
		_should_be_header_list.push_back("security");
		
		req.set_url(_source_url);
	}
	
	SHDUrl _source_url;
	std::list<std::string> _should_be_header_list;
	
	SHDRequest req;
};

TEST_F(SHDRequestTest, TestHeaderList)
{
	req.set_raw_header("signature", "my signature");
	req.set_raw_header("security", "none");
	
	const std::list<std::string> outList = req.raw_header_list();
	
	ASSERT_TRUE(outList.size() == _should_be_header_list.size() + 1 /* Host Header */) << "The list should have same size";
	
	EXPECT_STREQ("my signature", req.raw_header("signature").c_str());
	EXPECT_STREQ(req.raw_header("security").c_str(), "none");
	
	std::cout << req;
}



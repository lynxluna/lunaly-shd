/*
 *  SHDThreadTest.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/13/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

class SHDThreadTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		_valid_reqs.push_back(SHDRequest(SHDUrl("http://www.ansav.com/")));
		_valid_reqs.push_back(SHDRequest(SHDUrl( "http://www.hollywood-celebrity-pictures.com/"
												 "Celebrities/Keeley-Hazell/Keeley-Hazell-2.JPG")));
		_valid_reqs.push_back(SHDRequest(SHDUrl("http://www.digicelebs.com/cpg/albums/SC/Emily_Scott/"
							   "emily_scott_photoshoot_uhq_nude_ultra_sexy_04.jpg?auth=true")));
		_valid_reqs.push_back(SHDRequest(SHDUrl("http://blogfile.paran.com/BLOG_463639/200704/1176594807_sp_tokumoto05_006.jpg")));
		_valid_reqs.push_back(SHDRequest(SHDUrl("http://blog.joins.com/usr/1/1/115moon/1/3%28329%29.jpg")));
	}
	
	std::vector<SHDRequest> _valid_reqs;
	boost::asio::io_service io;
};
/*
TEST_F( SHDThreadTest, RequestTest )
{
	std::ofstream outfile1("/tmp/madness1.jpg", std::ofstream::binary );
	std::ofstream outfile2("/tmp/madness2.jpg", std::ofstream::binary );
	std::ofstream outfile3("/tmp/madness3.jpg", std::ofstream::binary );
	SHDThread t1( io ), t2( io ), t3( io ), t4(io);
	t1.call(_valid_reqs[1], &outfile1);
	t2.call(_valid_reqs[2], &outfile2);
	t3.call(_valid_reqs[3], &outfile3);
	t4.call(_valid_reqs[0], &std::cout);
	io.run();
	outfile1.close();
	outfile2.close();
	outfile3.close();
}


TEST_F( SHDThreadTest, MultiThreadTestImmediate )
{

	std::ofstream outfile1("/tmp/madness_alto1.jpg", std::ofstream::binary );
	std::ofstream outfile2("/tmp/madness_alto2.jpg", std::ofstream::binary );
	std::ofstream outfile3("/tmp/madness_alto3.jpg", std::ofstream::binary );
	
	SHDManager manager;
	manager.call( _valid_reqs[0], &std::cout, false );
	manager.call( _valid_reqs[1], &outfile1, false );
	manager.call( _valid_reqs[2], &outfile2, false );
	manager.call( _valid_reqs[3], &outfile3, false );
	
	manager.start();
	outfile1.close();
	outfile2.close();
	outfile3.close();
}
*/
TEST_F( SHDThreadTest, MultiThreadTestQueued )
{
	std::ofstream outfile1("/tmp/madness_abc1.html", std::ofstream::binary );
	std::ofstream outfile2("/tmp/madness_abc2.jpg", std::ofstream::binary );
	std::ofstream outfile3("/tmp/madness_abc3.jpg", std::ofstream::binary );
	std::ofstream outfile4("/tmp/madness_abc4.jpg", std::ofstream::binary );
	std::ofstream outfile5("/tmp/madness_abc5.jpg", std::ofstream::binary );
	
	
	SHDManager manager(2);
	manager.reset();
	manager.call( _valid_reqs[0], &outfile1 );
	manager.call( _valid_reqs[1], &outfile2 );
	//manager.call( _valid_reqs[2], &outfile3 );
	//manager.call( _valid_reqs[3], &outfile4 );
	manager.call( _valid_reqs[4], &outfile5 );
	
	manager.start();
	outfile1.close();
	outfile2.close();
	outfile3.close();
	outfile4.close();
	outfile5.close();
}

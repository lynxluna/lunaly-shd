/*
 *  GlobalSHDTest.h
 *  BSHD
 *
 *  Created by Lynx Luna on 4/17/10.
 *  Copyright 2010 MijiX Gemilang Persada. All rights reserved.
 *
 */

TEST(UtilTest, Base64EncodeTest)
{
	const std::string t("Aladdin:open sesame");
	const std::string outp = base64encode(reinterpret_cast<const unsigned char*> (t.c_str()), 
										  t.length());
	EXPECT_EQ( std::string("QWxhZGRpbjpvcGVuIHNlc2FtZQ=="), outp);
}

TEST(AuthTest, BasicAuthTest)
{
	SHDAuthSimpleRealm realm;
	SHDRequest req( SHDUrl( "http://irisresearch.library.cornell.edu/control/authBasic/authTest/") );
	realm.set_token("test");
	realm.set_token_secret("this");
	realm.sign_request(req);
	std::cout << req;
	boost::asio::io_service io;
	std::stringstream s;
	SHDThread t1(io);
	
	t1.call(req, &std::cout);
	
	io.run();
	
	// EXPECT_GT( s.str().length(), 0 );
}
#ifndef SRC_TESTS_TESTREDISSTRING_H_
#define SRC_TESTS_TESTREDISSTRING_H_

#include "TestComm.h"

// **************RedisString**************** //
typedef RedisClientTest RedisStringTest;
TEST_F(RedisStringTest, append)
{
	string value;
	ASSERT_TRUE(pClient->set("Time", "201601"));
	ASSERT_TRUE(pClient->append("Time", "23"));
	ASSERT_TRUE(pClient->get("Time", value));
	EXPECT_STREQ(value.c_str(), "20160123");

	ASSERT_TRUE(pClient->del("Time"));
	ASSERT_TRUE(pClient->append("Time", "20160124"));
	ASSERT_TRUE(pClient->get("Time", value));
	EXPECT_STREQ(value.c_str(), "20160124");
}

TEST_F(RedisStringTest, bitcount)
{
	//TODO: Need to implement test of bitcount
}

TEST_F(RedisStringTest, bitop)
{
	//TODO: Need to implement test of bitop
}

TEST_F(RedisStringTest, desc)
{
	int64_t value;
	ASSERT_TRUE(pClient->set("Num", "20"));
	ASSERT_TRUE(pClient->decr("Num", value));
	EXPECT_EQ(value, 19);
}

TEST_F(RedisStringTest, descby)
{
	int64_t value;
	ASSERT_TRUE(pClient->set("Num", "20"));
	ASSERT_TRUE(pClient->decrby("Num", 5, value));
	EXPECT_EQ(value, 15);
}

TEST_F(RedisStringTest, get)
{
	string value;
	ASSERT_TRUE(pClient->set("Time", "201601"));
	ASSERT_TRUE(pClient->get("Time", value));
	EXPECT_STREQ(value.c_str(), "201601");
}

TEST_F(RedisStringTest, getbit)
{
	//TODO: Need to implement test of getbit
}

TEST_F(RedisStringTest, getrange)
{
	string value;
	ASSERT_TRUE(pClient->set("Time", "20160123"));
	ASSERT_TRUE(pClient->getrange("Time", 4, 8, value));
	EXPECT_STREQ(value.c_str(), "0123");
}

TEST_F(RedisStringTest, getset)
{
	string value;
	ASSERT_TRUE(pClient->set("Time", "1024"));
	ASSERT_TRUE(pClient->getset("Time", "201601231024", value));
	EXPECT_STREQ(value.c_str(), "1024");
}

TEST_F(RedisStringTest, incr)
{
	int64_t value;
	ASSERT_TRUE(pClient->set("Num", "100"));
	ASSERT_TRUE(pClient->incr("Num", value));
	EXPECT_EQ(value, 101);
}

TEST_F(RedisStringTest, incrby)
{
	int64_t value;
	ASSERT_TRUE(pClient->set("Num", "100"));
	ASSERT_TRUE(pClient->incrby("Num", 20, value));
	EXPECT_EQ(value, 120);
}

TEST_F(RedisStringTest, incrbyfloat)
{
	double value;
	ASSERT_TRUE(pClient->set("Num", "100.24"));
	ASSERT_TRUE(pClient->incrbyfloat("Num", 1.11, value));
	EXPECT_EQ(value, 101.35);
}

TEST_F(RedisStringTest, mget)
{

}

#endif /* SRC_TESTS_TESTREDISSTRING_H_ */

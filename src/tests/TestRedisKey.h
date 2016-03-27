#ifndef SRC_TESTS_TESTREDISKEY_H_
#define SRC_TESTS_TESTREDISKEY_H_

#include "TestComm.h"
#include <ctime>

// **************RedisKeys**************** //
typedef RedisClientTest RedisKeyTest;
TEST_F(RedisKeyTest, exists)
{
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	EXPECT_TRUE(pClient->exists("Name"));
}

TEST_F(RedisKeyTest, del)
{
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	EXPECT_TRUE(pClient->exists("Name"));
	ASSERT_TRUE(pClient->del("Name"));
	EXPECT_FALSE(pClient->exists("Name"));
}

TEST_F(RedisKeyTest, expire)
{
	int64_t exp_time;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->expire("Name", 20));
	EXPECT_TRUE(pClient->ttl("Name", exp_time));
	EXPECT_LE(exp_time, 20);
}

TEST_F(RedisKeyTest, expireat)
{
	time_t now = time(NULL);
	int64_t exp_time = now + 20;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->expireat("Name", (int)exp_time));
	EXPECT_TRUE(pClient->ttl("Name", exp_time));
	EXPECT_LE(exp_time, 20);
}

TEST_F(RedisKeyTest, keys)
{
	vector<string> result;
	ASSERT_TRUE(pClient->flushdb());
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->keys("*", result));
	ASSERT_GE(result.size(), 1);
	EXPECT_STREQ(result[0].c_str(), "Name");
}

TEST_F(RedisKeyTest, persist)
{
	int64_t exp_time;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->expire("Name", 20));
	EXPECT_TRUE(pClient->persist("Name"));
	EXPECT_TRUE(pClient->ttl("Name", exp_time));
	EXPECT_EQ(exp_time, -1);
}

TEST_F(RedisKeyTest, pexpire)
{
	int64_t exp_time;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->pexpire("Name", 1000));
	EXPECT_TRUE(pClient->pttl("Name", exp_time));
	EXPECT_LE(exp_time, 1000);
}

TEST_F(RedisKeyTest, pexpireat)
{
	time_t now = time(NULL);
	int64_t exp_time = now + 20000;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->pexpireat("Name", int(exp_time)));
	EXPECT_TRUE(pClient->pttl("Name", exp_time));
	EXPECT_LE(exp_time, 20000);
}

TEST_F(RedisKeyTest, randomkey)
{
	pClient->flushdb();
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	string key;
	ASSERT_TRUE(pClient->randomkey(key));
	EXPECT_STREQ(key.c_str(), "Name");
}

TEST_F(RedisKeyTest, rename)
{
	string value;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->rename("Name", "NAME"));
	EXPECT_TRUE(pClient->get("NAME", value));
}

TEST_F(RedisKeyTest, renamenx)
{
	string value;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->set("NAME", "TEST"));
	EXPECT_FALSE(pClient->renamenx("Name", "NAME"));
	ASSERT_TRUE(pClient->del("NAME"));
	EXPECT_TRUE(pClient->renamenx("Name", "NAME"));
	EXPECT_TRUE(pClient->get("NAME", value));
}

TEST_F(RedisKeyTest, type)
{
	string strtype;
	ASSERT_TRUE(pClient->set("Name", "GLC"));
	ASSERT_TRUE(pClient->type("Name", strtype));
	EXPECT_STREQ(strtype.c_str(), "string");
}

#endif // SRC_TESTS_TESTREDISKEY_H_

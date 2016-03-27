#ifndef SRC_TESTS_TESTREDISCONN_H_
#define SRC_TESTS_TESTREDISCONN_H_

#include "TestComm.h"

// **************RedisConnection**************** //
typedef RedisClientTest RedisConnTest;
TEST_F(RedisConnTest, test_init){
	ASSERT_STREQ(teststr.c_str(), "start");
	ASSERT_TRUE(NULL != pRedisConn);
	ASSERT_TRUE(NULL != pClient);
}

TEST_F(RedisConnTest, select){
	EXPECT_TRUE(pRedisConn->select(0));
}

TEST_F(RedisConnTest, echo){
	string retstr = "";
	EXPECT_TRUE(pRedisConn->echo("Haha", retstr));
	ASSERT_STREQ("Haha", retstr.c_str());
}

TEST_F(RedisConnTest, ping){
	EXPECT_TRUE(pRedisConn->ping());
}

TEST_F(RedisConnTest, setname_getname){
	ASSERT_TRUE(pRedisConn->client_setname("TestConn"));
	string name = "";
	ASSERT_TRUE(pRedisConn->client_getname(name));
	EXPECT_STREQ("TestConn", name.c_str());
}

// Quit测试需要放在最后，否则连接会丢失
TEST_F(RedisConnTest, quit){
	ASSERT_TRUE(pRedisConn->quit());
}

// **************RedisServer**************** //
typedef RedisClientTest RedisSCmdTest;
TEST_F(RedisSCmdTest, test_init){
	ASSERT_STREQ(teststr.c_str(), "start");
	ASSERT_TRUE(NULL != pRedisConn);
	ASSERT_TRUE(NULL != pClient);
}

TEST_F(RedisSCmdTest, config_get)
{
	ArrayReply replay_data;
	ASSERT_TRUE(pClient->config_get("port", replay_data));
	ASSERT_EQ(replay_data.size(), 2);
	EXPECT_EQ(replay_data[1].type, REPLY_STRING);
	char port[10] = "";
	sprintf(port, "%d", PORT);
	EXPECT_STREQ(replay_data[1].str.c_str(), port);
}

TEST_F(RedisSCmdTest, config_set)
{
	ArrayReply replay_data;
	ASSERT_TRUE(pClient->config_set("loglevel", "debug"));
	ASSERT_TRUE(pClient->config_get("loglevel", replay_data));
	ASSERT_EQ(replay_data.size(), 2);
	EXPECT_EQ(replay_data[1].type, REPLY_STRING);
	EXPECT_STREQ(replay_data[1].str.c_str(), "debug");
}

TEST_F(RedisSCmdTest, flushdb)
{
	EXPECT_TRUE(pClient->flushdb());
}

TEST_F(RedisSCmdTest, dbsize)
{
	int64_t size = 0;
	ASSERT_TRUE(pClient->flushdb());
	EXPECT_TRUE(pClient->dbsize(size));
	EXPECT_EQ(size, 0);
}

TEST_F(RedisSCmdTest, save)
{
	ASSERT_TRUE(pClient->save());
}

TEST_F(RedisSCmdTest, time)
{
	string timestamp;
	ASSERT_TRUE(pClient->time(timestamp));
}

#endif //SRC_TESTS_TESTREDISCONN_H_

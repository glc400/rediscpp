#ifndef SRC_TESTS_TESTCOMM_H_
#define SRC_TESTS_TESTCOMM_H_

#include "gtest/gtest.h"
#include "RedisClient.h"

const char* HOST = "127.0.0.1";
const int PORT = 6880;
enum REPLY_TYPE{REPLY_STRING=1, REPLY_ARRAY, REPLY_INTEGER, REPLY_NIL, REPLY_STATUS, REPLY_ERROR};
char szReplyType[7][20] = {"", "STRING", "ARRAY", "INTEGER", "NIL", "STATUS", "ERROR"};

using namespace std;

class RedisClientTest : public ::testing::Test{
protected:
  virtual void SetUp() {
	teststr = "start";
	pClient = new RedisClient("127.0.0.1", 6880, 0, "", 0, 1, 5);
	pRedisConn = pClient->GetConnection();
  }

  virtual void TearDown(){
	  pClient->FreeConnection(pRedisConn);
	  pRedisConn = NULL;
	  delete pClient;
	  pClient = NULL;
  }

  string teststr;
  RedisClient* pClient;
  RedisConnection* pRedisConn;
};

#endif // SRC_TESTS_TESTCOMM_H_

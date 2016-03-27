#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include "hiredis.h"

const char* host = "127.0.0.1";
const int port = 6890;
enum REPLY_TYPE{REPLY_STRING=1, REPLY_ARRAY, REPLY_INTEGER, REPLY_NIL, REPLY_STATUS, REPLY_ERROR};

void print_reply(redisReply* pRedisReply, int nType)
{
	char szReply[1024] = "";
	char szPrefix[10] = "---";
	if (1 == nType) strcpy(szPrefix, "------");
	char szReplyType[6][20] = {"STRING", "ARRAY", "INTEGER", "NIL", "STATUS", "ERROR"};
	printf("%sReply->type = %s\n", szPrefix, szReplyType[pRedisReply->type-1]);
	strncpy(szReply, pRedisReply->str, sizeof(szReply));
	printf("%sReply->str = %s\n", szPrefix, szReply);
	printf("%sReply->integer = %ld\n", szPrefix, pRedisReply->integer);
}

int main(int argc, char** argv)
{
	redisContext* pRedisCtx;
	redisReply* pRedisReply;
	char szCmd[1024] = "";

	if(argc != 2)
	{
		printf("Usage: %s command\n", argv[0]);
		return 0;
	}
	strcpy(szCmd, argv[1]);

	pRedisCtx = redisConnect(host, port);
	if(NULL == pRedisCtx || pRedisCtx->err)
	{
		printf("Connect to redis error: %s\n", pRedisCtx->errstr);
		return -1;
	}
	printf("Connection is ready!\n");

	printf("Redis command = [%s]\n", szCmd);
	pRedisReply = (redisReply*)redisCommand(pRedisCtx, szCmd);
	if(NULL == pRedisReply || pRedisReply->type == REPLY_ERROR)
	{
		printf("Redis reply is error: %s\n", pRedisReply->str);
		if (NULL != pRedisReply) freeReplyObject(pRedisReply);
		return -2;

	}

	printf("--Reply main info:\n");
	print_reply(pRedisReply, 0);
    for (size_t i = 0; i < pRedisReply->elements; i++)
    {
    	if (0 == i) printf("---Reply elements info:\n");
    	printf("----Reply->elements[%d]:\n", i);
    	print_reply(pRedisReply->element[i], 1);
    }

    if (NULL != pRedisReply) freeReplyObject(pRedisReply);
	return 0;
}


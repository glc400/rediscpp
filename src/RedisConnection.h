#ifndef REDIS_CONNECTION_H
#define REDIS_CONNECTION_H


#include "hiredis.h"
#include "RedisDefine.h"
#include "RedisSynch.h"
#include "RedisException.h"

using namespace std;

class RedisPool;
class RedisCommand;

class RedisConnection
{
public:
	RedisConnection(RedisPool* pRedisPool);
	~RedisConnection();

	bool Connect();
	void FreeContext();

	static bool CheckReply(const redisReply *pReply);
	static void FreeReply(const redisReply *pReply);
	redisContext* GetRedisCtx() { return m_pRedisCtx; }
	char* GetErrorInfo();

	bool auth(const char* passwd);
	bool select(unsigned int db);
	bool echo(const string& str, string& retstr);
	bool ping();
	bool quit();
	bool client_getname(string& name);
	bool client_setname(const string& name);

private:
	redisContext* m_pRedisCtx;
	long m_lLastActiveTime;
	RedisPool* m_pRedisPool;
};

#endif /* REDIS_CONNECTION_H */

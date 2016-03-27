#ifndef REDIS_POOL_H
#define REDIS_POOL_H

#include "RedisConnection.h"

class RedisPool
{
public:
	RedisPool(const std::string host = "localhost",
			unsigned int port = 6379,
			unsigned int db = 0,
			const std::string passwd = "",
			unsigned int timeout = 0,
			unsigned int minconn = 1,
			unsigned int maxconn = 2);
	~RedisPool();

	int Init();
	RedisPool* FromUrl(const std::string url);
	static void* ServerCron(void* args);
	const char* GetServerHost() { return m_strHost.c_str(); }
	int GetServerPort() { return m_nPort; }
	int GetDB() { return m_nDB; }
	const char* GetPasswd() { return m_strPass.c_str(); }
	int GetTimeOut() { return m_nTimeOut; }

    RedisConnection* GetConnection();
    void FreeConnection(RedisConnection*);
private:
	const std::string m_strHost;
	unsigned int m_nPort;
	unsigned int m_nDB;
	const std::string m_strPass;
	unsigned int m_nTimeOut;  //ms
	unsigned int m_nMinConn;
	unsigned int m_nMaxConn;
	mutable MutexLock m_cMutex;
	pthread_t m_pThreadId;
	Condition m_cCond;
	std::list<RedisConnection*>	m_listConnections;
	bool m_bQuit;
};


#endif // REDIS_POOL_H

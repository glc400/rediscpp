#include "RedisClient.h"


RedisClient::RedisClient(const std::string host,
		unsigned int port,
		unsigned int db,
		const std::string passwd,
		unsigned int timeout,
		unsigned int minconn,
		unsigned int maxconn)
{
	m_pRedisPool = new RedisPool(host, port, db, passwd, timeout, minconn, maxconn);
	m_pRedisPool->Init();
	m_pRedisCmd = new RedisCommand(m_pRedisPool);
}

RedisClient::RedisClient(RedisPool* pRedisPool)
{
	m_pRedisPool = pRedisPool;
	m_pRedisPool->Init();
}

RedisClient::RedisClient(const std::string url)
{
	// TODO: 这块实现使用url初始化一个连接池
	// redis://:password@localhost:6379/0
	m_pRedisPool = new RedisPool();
	RedisClient(m_pRedisPool);
}

RedisClient::~RedisClient()
{
	if (m_pRedisPool)
	{
		delete m_pRedisPool;
		m_pRedisPool = NULL;
	}

	if (m_pRedisCmd)
	{
		delete m_pRedisCmd;
		m_pRedisCmd = NULL;
	}
}

RedisPool* RedisClient::GetRedisPool()
{
	return m_pRedisPool;
}

RedisConnection* RedisClient::GetConnection()
{
	RedisConnection* pRedisConn = m_pRedisPool->GetConnection();
	return pRedisConn;
}

void RedisClient::FreeConnection(RedisConnection* pRedisConn)
{
	m_pRedisPool->FreeConnection(pRedisConn);
	return;
}


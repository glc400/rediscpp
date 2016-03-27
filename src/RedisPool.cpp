#include "RedisPool.h"

RedisPool::RedisPool(const std::string host,
		unsigned int port,
		unsigned int db,
		const std::string passwd,
		unsigned int timeout,
		unsigned int minconn,
		unsigned int maxconn)
	: m_strHost(host),
	  m_nPort(port),
	  m_nDB(db),
	  m_strPass(passwd),
	  m_nTimeOut(timeout),
	  m_nMinConn(minconn),
	  m_nMaxConn(maxconn),
	  m_cMutex(),
	  m_cCond(m_cMutex),
	  m_listConnections(),
	  m_bQuit(false)
{
	
}

RedisPool* RedisPool::FromUrl(const std::string url)
{
	RedisPool* pRedisPool = new RedisPool();
	return pRedisPool;
}

RedisPool::~RedisPool()
{
	m_bQuit = true;
	if(pthread_join(m_pThreadId, NULL))
	{
		std::cout << "Thread quit failed!" << std::endl;
	}

	MutexLockGuard lock(m_cMutex);
	for (std::list<RedisConnection*>::iterator it = m_listConnections.begin();
			it != m_listConnections.end(); it++)
	{
		delete *it;
	}

	m_listConnections.clear();
	m_nMinConn= 0;
}

int RedisPool::Init()
{
	for (int i = 0; i < m_nMinConn; i++)
	{
		RedisConnection* pRedisConn = new RedisConnection(this);
		try
		{
			pRedisConn->Connect();
		}
		catch(RedisException& ex)
		{
			cout << "Init() -- " << ex.what() << endl;
			if (pRedisConn)
			{
				delete pRedisConn;
				pRedisConn = NULL;
			}
		}
		
		if (pRedisConn != NULL)
			m_listConnections.push_back(pRedisConn);
	}

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
	if(pthread_create(&m_pThreadId, &attr, ServerCron, (void*)this) != 0)
	{
		std::cout << "pthread_create failed" << std::endl;
		return -1;
	}

	return 0;
}

void * RedisPool::ServerCron(void* arg)
{
	RedisPool *pRedisPool = (RedisPool*)arg;

	while (!pRedisPool->m_bQuit)
	{
		pRedisPool->m_cCond.WaitForSeconds(SLEEP_SECONDS);
		MutexLockGuard lock(pRedisPool->m_cMutex);

		std::list<RedisConnection*>::iterator it = pRedisPool->m_listConnections.begin();
		for (; it != pRedisPool->m_listConnections.end(); )
		{
			if ((*it)->ping() == false)
			{
				delete *it;
				pRedisPool->m_listConnections.remove(*it++);
			}
			else
			{
				it++;
			}
		}
	}

	return (void*)NULL;
}

RedisConnection* RedisPool::GetConnection()
{
	MutexLockGuard lock(m_cMutex);

	while (m_listConnections.empty())
	{
		if (m_nMinConn >= m_nMaxConn)
		{
			cout << "Now to wait .." << endl;
			m_cCond.Wait();
		} 
		else 
		{
			RedisConnection* pRedisConn = new RedisConnection(this);
			try
			{
				pRedisConn->Connect();
			}
			catch(RedisException& exc)
			{
				cout << "GetConnection() -- " << exc.what() << endl;
				if (pRedisConn)
				{
					delete pRedisConn;
					pRedisConn = NULL;
				}
				return NULL;
			}
			m_listConnections.push_back(pRedisConn);
			m_nMinConn++;
		}
	}

	RedisConnection* pRedisConn = m_listConnections.front();
	m_listConnections.pop_front();

	return pRedisConn;
}

void RedisPool::FreeConnection(RedisConnection* pRedisConn)
{
	MutexLockGuard lock(m_cMutex);

	std::list<RedisConnection*>::iterator it = m_listConnections.begin();
	for (; it != m_listConnections.end(); it++)
	{
		if (*it == pRedisConn)
			break;
	}

	if (it == m_listConnections.end())
	{
		m_listConnections.push_back(pRedisConn);
	}

	m_cCond.Notify();
}



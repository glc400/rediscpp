#include "RedisCommand.h"

#define PREPARE_REDIS_CONTEXT_EXCEPTION(message)  std::string(__FUNCTION__)+ \
									"|"+ (m_pRedisCtx->errstr == NULL ? "" : m_pRedisCtx->errstr)+ \
									"|" + std::string(message)

#define PREPARE_REDIS_EXCEPTION(message)  std::string(__FUNCTION__)+ \
									"|"+ (m_pRedisCtx->errstr == NULL ? "" : m_pRedisCtx->errstr)+ \
									"|" +(pReply->str == NULL ? "" : pReply->str)+ \
									"|" + std::string(message)

RedisConnection::RedisConnection(RedisPool* pRedisPool)
{
	m_pRedisCtx = NULL;
	m_lLastActiveTime = time(NULL);
	m_pRedisPool = pRedisPool;
}

RedisConnection::~RedisConnection()
{
	FreeContext();
}

void RedisConnection::FreeContext()
{
	if (m_pRedisCtx)
	{
		redisFree(m_pRedisCtx);
		m_pRedisCtx = NULL;
	}
}

char* RedisConnection::GetErrorInfo()
{
	return RedisCommand::GetErrInfo();
}

bool RedisConnection::Connect()
{
	// ³¬Ê±Á¬½ÓÅÐ¶Ï
	if(0 == m_pRedisPool->GetTimeOut()){
		m_pRedisCtx = redisConnect(m_pRedisPool->GetServerHost(), m_pRedisPool->GetServerPort());
	}else{
		struct timeval timeout = {0, m_pRedisPool->GetTimeOut()*1000};
		m_pRedisCtx = redisConnectWithTimeout(m_pRedisPool->GetServerHost(), m_pRedisPool->GetServerPort(), timeout);
	}

	if (NULL == m_pRedisCtx || m_pRedisCtx->err){
		RedisException exc(PREPARE_REDIS_CONTEXT_EXCEPTION("Connect failed!"));
		FreeContext();
		throw exc;
	}

	// ÃÜÂë·ÃÎÊÅÐ¶Ï
	if(0 != strlen(m_pRedisPool->GetPasswd())){
		if(!auth(m_pRedisPool->GetPasswd())){
			RedisException exc(PREPARE_REDIS_CONTEXT_EXCEPTION("Auth to connection failed!"));
			FreeContext();
			throw exc;
		}
	}

	// ÇÐ»»Êý¾Ý¿â
	if(!select(m_pRedisPool->GetDB())){
		RedisException exc(PREPARE_REDIS_CONTEXT_EXCEPTION("Change database " +
				toString(m_pRedisPool->GetDB()) + " failed!"));
		FreeContext();
		throw exc;
	}

	return 0;
}

bool RedisConnection::CheckReply(const redisReply* pReply)
{
	if(pReply == NULL)
		return false;

	switch(pReply->type)
	{
	case REDIS_REPLY_STRING:
		return true;
	case REDIS_REPLY_ARRAY:
		return true;
	case REDIS_REPLY_INTEGER:
		return true;
	case REDIS_REPLY_NIL:
		return false;
	case REDIS_REPLY_STATUS:
		return (strcasecmp(pReply->str, "OK") == 0) ? true : false;
	case REDIS_REPLY_ERROR:
		return false;
	default:
		return false;
	}

	return false;
}

void RedisConnection::FreeReply(const redisReply* pReply)
{
	if (pReply)
	{
		freeReplyObject((void*)pReply);
		pReply = NULL;
	}
}

bool RedisConnection::auth(const char* passwd)
{
	return RedisCommand::command_bool(m_pRedisCtx, "AUTH %s", passwd);
}

bool RedisConnection::select(unsigned int db)
{
	return RedisCommand::command_bool(m_pRedisCtx, "SELECT %d", db);
}

bool RedisConnection::echo(const string& str, string& retstr)
{
	return RedisCommand::command_string(m_pRedisCtx, retstr, "ECHO %s", str.c_str());
}

bool RedisConnection::ping()
{
	return RedisCommand::command_bool(m_pRedisCtx, "PING");
}

bool RedisConnection::quit()
{
	return RedisCommand::command_bool(m_pRedisCtx, "QUIT");
}

bool RedisConnection::client_getname(string& name)
{
	return RedisCommand::command_string(m_pRedisCtx, name, "CLIENT GETNAME");
}

bool RedisConnection::client_setname(const string& name)
{
	return RedisCommand::command_bool(m_pRedisCtx, "CLIENT SETNAME %s", name.c_str());
}

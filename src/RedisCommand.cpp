#include "RedisCommand.h"
#include <stdio.h>

char RedisCommand::m_strError[LEN_ERROR_INFO+1] = "";

RedisCommand::RedisCommand(RedisPool* pRedisPool)
{
	m_pRedisPool = pRedisPool;
}

RedisCommand::~RedisCommand()
{

}

bool RedisCommand::SetErrInfo(const char* info, int len)
{
    if (NULL == info)
        return false;
    len = (len > LEN_ERROR_INFO ? LEN_ERROR_INFO : len);
    strncpy(m_strError, info, len);
    m_strError[len] = '\0';
    return true;
}

void RedisCommand::SetErrMsg(const char* fmt, ...)
{
    char szBuf[LEN_ERROR_INFO] = {0};
    va_list va;
    va_start(va, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, va);
    va_end(va);

    RedisCommand::SetErrInfo(szBuf, strlen(szBuf));
}

bool RedisCommand::SetErrInfo(void *p)
{
    if (NULL == p)
    	return false;

    redisReply* pReply = (redisReply*)p;
    RedisCommand::SetErrInfo(pReply->str, pReply->len);

    return true;
}

redisReply* RedisCommand::command(const char* cmd)
{
    RedisConnection* pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return NULL;
    }

    redisReply* pReply = static_cast<redisReply*>(redisCommand(pRedisConn->GetRedisCtx(), cmd));
    m_pRedisPool->FreeConnection(pRedisConn);
    return pReply;
}

bool RedisCommand::command_bool(const char *cmd, ...)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply* pReply = static_cast<redisReply*>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply)) {
        if (REDIS_REPLY_STATUS == pReply->type) {
            bRet = true;
        }else {
            bRet = (pReply->integer == 1) ? true : false;
        }
    } else {
    	RedisCommand::SetErrInfo(pReply->str, pReply->len);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::command_bool(redisContext* pRedisCtx, const char *cmd, ...)
{
	bool bRet = false;
	if(NULL == pRedisCtx)
	{
    	RedisCommand::SetErrInfo(ERROR_NULL_CONTEXT, strlen(ERROR_NULL_CONTEXT));
        return bRet;
	}

    va_list args;
    va_start(args, cmd);
    redisReply* pReply = static_cast<redisReply*>(redisvCommand(pRedisCtx, cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply)){
    	bRet = true;
    }else{
    	// Bgu fix: pReply of PING type = status but str is PONG(isn't OK)
    	if(strcasecmp(cmd, "PING") == 0 && strcasecmp(pReply->str, "PONG") == 0){
    		bRet = true;
    	}else{
    		RedisCommand::SetErrInfo(pReply);
    	}
    }

    RedisConnection::FreeReply(pReply);
    return bRet;
}

bool RedisCommand::command_string(redisContext* pRedisCtx, string& retstr, const char *cmd, ...)
{
	bool bRet = false;
	if(NULL == pRedisCtx)
	{
    	RedisCommand::SetErrInfo(ERROR_NULL_CONTEXT, strlen(ERROR_NULL_CONTEXT));
        return false;
	}

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply*>(redisvCommand(pRedisCtx, cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply))
    {
    	retstr.assign(pReply->str, pReply->len);
        bRet = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    return bRet;
}

bool RedisCommand::command_status(const char* cmd, ...)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
        SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply*>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply)){
        bRet = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::command_integer(int64_t &retval, const char* cmd, ...)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply*>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply)){
        retval = pReply->integer;
        bRet = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::command_string(string& retstr, const char* cmd, ...)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply*>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply))
    {
    	retstr.assign(pReply->str, pReply->len);
        bRet = true;
    } else {
    	// Bgu fix: pReply of TYPE type = status but str isn't OK
    	if(strncmp(cmd, "TYPE", 4) == 0 && inString(pReply->str, "none string list set zset hash")){
    		retstr.assign(pReply->str, pReply->len);
    		bRet = true;
    	}else{
    		RedisCommand::SetErrInfo(pReply);
    	}
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::command_list(VDATA& vData, const char* cmd, ...)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply*>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply))
    {
        for (size_t i = 0; i < pReply->elements; i++)
        {
        	vData.push_back(std::string(pReply->element[i]->str, pReply->element[i]->len));
        }
        bRet  = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::command_array(ArrayReply& array, const char* cmd, ...)
{
    bool bRet = false;
    RedisConnection* pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    va_list args;
    va_start(args, cmd);
    redisReply *pReply = static_cast<redisReply *>(redisvCommand(pRedisConn->GetRedisCtx(), cmd, args));
    va_end(args);
    if (RedisConnection::CheckReply(pReply))
    {
        for (size_t i = 0; i < pReply->elements; i++)
        {
            TReplyItem item;
            item.type = pReply->element[i]->type;
            item.str.assign(pReply->element[i]->str, pReply->element[i]->len);
            array.push_back(item);
        }
        bRet  = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);
    return bRet;
}

bool RedisCommand::commandargv_bool(VDATA& vData)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    std::vector<const char*>argv(vData.size());
    std::vector<size_t> argvlen(vData.size());
    unsigned int j = 0;
    for (VDATA::const_iterator it = vData.begin(); it != vData.end(); ++it, ++j)
    {
        argv[j] = it->c_str();
        argvlen[j] = it->size();
    }

    redisReply *pReply = static_cast<redisReply *>(redisCommandArgv(pRedisConn->GetRedisCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (RedisConnection::CheckReply(pReply))
    {
        bRet = (1 == pReply->integer) ? true:false;
    }
    else
    {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::commandargv_status(const VDATA& vData)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    std::vector<const char*> argv(vData.size());
    std::vector<size_t> argvlen(vData.size());
    unsigned int j = 0;
    for (VDATA::const_iterator it = vData.begin(); it != vData.end(); ++it, ++j)
    {
        argv[j] = it->c_str();
        argvlen[j] = it->size();
    }

    redisReply *pReply = static_cast<redisReply *>(redisCommandArgv(pRedisConn->GetRedisCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (RedisConnection::CheckReply(pReply)) {
        bRet = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}

bool RedisCommand::commandargv_array(const VDATA& vDataIn, ArrayReply& array)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    std::vector<const char*> argv(vDataIn.size());
    std::vector<size_t> argvlen(vDataIn.size());
    unsigned int j = 0;
    for (VDATA::const_iterator it = vDataIn.begin(); it != vDataIn.end(); ++it, ++j )
    {
        argv[j] = it->c_str();
        argvlen[j] = it->size();
    }

    redisReply *pReply = static_cast<redisReply *>(redisCommandArgv(pRedisConn->GetRedisCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (RedisConnection::CheckReply(pReply))
    {
        for (size_t i = 0; i<pReply->elements; i++)
        {
            TReplyItem item;
            item.type = pReply->element[i]->type;
            item.str.assign(pReply->element[i]->str, pReply->element[i]->len);
            array.push_back(item);
        }
        bRet  = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);
    return bRet;
}

bool RedisCommand::commandargv_array(const VDATA& vDataIn, VALUES& array)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    std::vector<const char*> argv(vDataIn.size());
    std::vector<size_t> argvlen(vDataIn.size());
    unsigned int j = 0;
    for (VDATA::const_iterator it = vDataIn.begin(); it != vDataIn.end(); ++it, ++j)
    {
        argv[j] = it->c_str();
        argvlen[j] = it->size();
    }

    redisReply *pReply = static_cast<redisReply*>(redisCommandArgv(pRedisConn->GetRedisCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (RedisConnection::CheckReply(pReply))
    {
        for (size_t i = 0; i < pReply->elements; i++)
        {
            std::string str(pReply->element[i]->str, pReply->element[i]->len);
            array.push_back(str);
        }
        bRet  = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);
    return bRet;
}

bool RedisCommand::commandargv_integer(const VDATA& vDataIn, int64_t& retval)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    std::vector<const char*> argv(vDataIn.size());
    std::vector<size_t> argvlen(vDataIn.size());
    unsigned int j = 0;
    for (VDATA::const_iterator it = vDataIn.begin(); it != vDataIn.end(); ++it, ++j )
    {
        argv[j] = it->c_str();
        argvlen[j] = it->size();
    }

    redisReply* pReply = static_cast<redisReply*>(redisCommandArgv(pRedisConn->GetRedisCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (RedisConnection::CheckReply(pReply))
    {
        retval = pReply->integer;
        bRet  = true;
    } else {
    	RedisCommand::SetErrInfo(pReply);
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);
    return bRet;
}

bool RedisCommand::command_append(const char* cmd, const vector<string>& keys, vector<TReplyItem>& vReply)
{
    bool bRet = false;
    RedisConnection *pRedisConn = m_pRedisPool->GetConnection();
    if (NULL == pRedisConn)
    {
    	RedisCommand::SetErrInfo(ERROR_GET_CONNECT, strlen(ERROR_GET_CONNECT));
        return false;
    }

    int setnum = 0;
    redisContext* pRedisCtx = pRedisConn->GetRedisCtx();
    vector<string>::const_iterator iter = keys.begin();
    for(; iter != keys.end(); iter++, setnum++)
    {
    	const string &key = *iter;
    	if(REDIS_OK != redisAppendCommand(pRedisCtx, "%s %s", cmd, key.c_str())){
    		RedisCommand::SetErrInfo(pRedisCtx->errstr, strlen(pRedisCtx->errstr));
    		bRet = false;
    		break;
    	}
    }

    redisReply *pReply = NULL;
    while(bRet && (setnum-- > 0))
    {
        if (REDIS_OK != redisGetReply(pRedisCtx, (void**)&pReply)){
    		RedisCommand::SetErrInfo(pRedisCtx->errstr, strlen(pRedisCtx->errstr));
    		bRet = false;
    		break;
        }

        if (RedisConnection::CheckReply(pReply))
        {
            for (size_t i = 0; i<pReply->elements; i++)
            {
                TReplyItem item;
                item.type = pReply->element[i]->type;
                item.str.assign(pReply->element[i]->str, pReply->element[i]->len);
                vReply.push_back(item);
            }
        } else {
        	RedisCommand::SetErrInfo(pReply);
        	bRet = false;
        	break;
        }
    }

    RedisConnection::FreeReply(pReply);
    m_pRedisPool->FreeConnection(pRedisConn);

    return bRet;
}


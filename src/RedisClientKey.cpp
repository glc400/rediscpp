#include "RedisClient.h"
#include <string.h>

bool RedisClient::del(const string& key)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "del");
        return false;
    }

    return m_pRedisCmd->command_bool("DEL %s", key.c_str());
}

bool RedisClient::del(const vector<string>& vkey, int64_t& count)
{
    count = 0;
    vector<string>::const_iterator it = vkey.begin();
    for(; it!=vkey.end(); ++it){
        const string &key = (*it);
        if (del(key))
            count++;
    }
    return true;
}

bool RedisClient::dump(const string& key, string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "dump");
        return false;
    }

    return m_pRedisCmd->command_string(value, "DUMP %s", key.c_str());
}

bool RedisClient::exists(const string& key)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "exists");
        return false;
    }

    return m_pRedisCmd->command_bool("EXISTS %s", key.c_str());
}

bool RedisClient::expire(const string& key, unsigned int second)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "expire");
        return false;
    }

    int64_t ret = -1;
    if (!m_pRedisCmd->command_integer(ret, "EXPIRE %s %u", key.c_str(), second)){
        return false;
    }

    if (1 != ret){
        m_pRedisCmd->SetErrMsg("EXPIRE return %ld ", ret);
        return false;
    }

    return true;
}

bool RedisClient::expireat(const string& key, unsigned int timestamp)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "expireat");
        return false;
    }

    return m_pRedisCmd->command_bool("EXPIREAT %s %u", key.c_str(), timestamp);
}

bool RedisClient::keys(const string& pattern, vector<string>& vResult)
{
    return m_pRedisCmd->command_list(vResult, "KEYS %s", pattern.c_str());
}

bool RedisClient::persist(const string& key)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "persist");
        return false;
    }

    return m_pRedisCmd->command_bool("PERSIST %s", key.c_str());
}

bool RedisClient::pexpire(const string& key, unsigned int milliseconds)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "pexpire");
        return false;
    }

    return m_pRedisCmd->command_bool("PEXPIRE %s %u", key.c_str(), milliseconds);
}

bool RedisClient::pexpireat(const string& key, unsigned int millisecondstimestamp)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "pexpireat");
        return false;
    }

    return m_pRedisCmd->command_bool("PEXPIREAT %s %u", key.c_str(), millisecondstimestamp);
}

bool RedisClient::pttl(const string& key, int64_t &milliseconds)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "pttl");
        return false;
    }

    return m_pRedisCmd->command_integer(milliseconds, "PTTL %s", key.c_str());
}

bool RedisClient::randomkey(string& key)
{
    return m_pRedisCmd->command_string(key, "RANDOMKEY");
}

bool RedisClient::rename(const string& key, const string& newkey)
{
    if (0 == key.length() || 0 == newkey.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "rename");
        return false;
    }

    return m_pRedisCmd->command_status("RENAME %s %s", key.c_str(), newkey.c_str());
}

bool RedisClient::renamenx(const string& key, const string& newkey)
{
    if (0 == key.length() || 0 == newkey.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "renamenx");
        return false;
    }

    int64_t ret = -1;
    if (!m_pRedisCmd->command_integer(ret, "RENAMENX %s %s", key.c_str(), newkey.c_str())){
        return false;
    }

    if (1 != ret){
        m_pRedisCmd->SetErrMsg("RENAMENX return %ld ", ret);
        return false;
    }

    return true;
}

bool RedisClient::ttl(const string& key, int64_t &seconds)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "ttl");
        return false;
    }

    return m_pRedisCmd->command_integer(seconds, "TTL %s", key.c_str());
}

bool RedisClient::type(const string& key, string& retstr)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "type");
        return false;
    }

    return m_pRedisCmd->command_string(retstr, "TYPE %s", key.c_str());
}

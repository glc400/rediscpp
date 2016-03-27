#include "RedisClient.h"

bool RedisClient::lindex(const string& key, int64_t index, string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lindex");
        return false;
    }

    return m_pRedisCmd->command_string(value, "LINDEX %s %lld", key.c_str(), index);
}

bool RedisClient::linsert(const string& key, const ELIST_MODE mod, const string& pivot, const string& value, int64_t& retval)
{
    static const char* lmodel[2] = {"BEFORE", "AFTER"};
    if (0 == key.length() || mod > AFTER){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "linsert");
        return false;
    }

    return m_pRedisCmd->command_integer(retval, "LINSERT %s %s %s %s", key.c_str(), lmodel[mod], pivot.c_str(), value.c_str());
}

bool RedisClient::llen(const string& key, int64_t& retval)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "llen");
        return false;
    }

    return m_pRedisCmd->command_integer(retval, "LLEN %s", key.c_str());
}

bool RedisClient::lpop(const string& key, string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lpop");
        return false;
    }

    return m_pRedisCmd->command_string(value, "LPOP %s", key.c_str());
}

bool RedisClient::lpush(const string& key, const vector<string>& vValue, int64_t& length)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lpush");
        return false;
    }

    vector<string> vCmdData;
    vCmdData.push_back("LPUSH");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vValue);

    return m_pRedisCmd->commandargv_integer(vCmdData, length);
}

bool RedisClient::lpushx(const string& key, string& value, int64_t& length)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lpushx");
        return false;
    }

    return m_pRedisCmd->command_integer(length, "LPUSHX %s %s", key.c_str(), value.c_str());
}

bool RedisClient::lrange(const string& key, int64_t start, int64_t end, ArrayReply& array)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lrange");
        return false;
    }

	return m_pRedisCmd->command_array(array, "LRANGE %s %lld %lld", key.c_str(), start, end);
}

bool RedisClient::lrem(const string& key, int count, const string& value, int64_t num)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lrem");
        return false;
    }

    return m_pRedisCmd->command_integer(num, "LREM %s %d %s", key.c_str(), count, value.c_str());
}

bool RedisClient::lset(const string& key, int index, const string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "lset");
        return false;
    }

    return m_pRedisCmd->command_status("LSET %s %d %s", key.c_str(), index, value.c_str());
}

bool RedisClient::ltrim(const string& key, int start, int end)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "ltrim");
        return false;
    }

    return m_pRedisCmd->command_status("LTRIM %s %d %d", key.c_str(), start, end);
}

bool RedisClient::rpop(const string& key, string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "rpop");
        return false;
    }

    return m_pRedisCmd->command_string(value, "RPOP %s", key.c_str());
}

bool RedisClient::rpoplpush(const string& key_src, const string& key_dest, string& value)
{
    if (0 == key_src.length() || 0==key_dest.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "rpoplpush");
        return false;
    }

    return m_pRedisCmd->command_string(value, "RPOPLPUSH %s %s", key_src.c_str(), key_dest.c_str());
}

bool RedisClient::rpush(const string& key, const vector<string>& vValue, int64_t& length)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "rpush");
        return false;
    }

    vector<string> vCmdData;
    vCmdData.push_back("RPUSH");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vValue);

    return m_pRedisCmd->commandargv_integer(vCmdData, length);
}

bool RedisClient::rpushx(const string& key, const string& value, int64_t& length)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "rpushx");
        return false;
    }

    return m_pRedisCmd->command_integer(length, "RPUSHX %s %s", key.c_str(), value.c_str());
}

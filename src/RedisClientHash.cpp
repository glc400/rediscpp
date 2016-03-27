#include "RedisClient.h"


bool RedisClient::hdel(const string& key, const string& field, int64_t& count)
{
    return m_pRedisCmd->command_integer(count, "HDEL %s %s %s", key.c_str(), field.c_str());
}

bool RedisClient::hdel(const string& key, const vector<string>& vfiled, int64_t& count)
{
	vector<string> vCmdData;
    vCmdData.push_back("HDEL");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vfiled);
    return m_pRedisCmd->commandargv_integer(vCmdData, count);
}

bool RedisClient::hexist(const string& key, const string& field)
{
    return m_pRedisCmd->command_bool("HEXIST %s %s %s", key.c_str(), field.c_str());
}

bool RedisClient::hget(const string& key, const string& field, string& value)
{
    return m_pRedisCmd->command_string(value, "HGET %s %s", key.c_str(), field.c_str());
}

bool RedisClient::hgetall( const string& key, ArrayReply& array)
{
    return m_pRedisCmd->command_array(array, "HGETALL %s", key.c_str());
}

bool RedisClient::hincrby(const string& key, const string& field, int64_t increment, int64_t& num )
{
    return m_pRedisCmd->command_integer(num, "HINCRBY %s %s %lld", key.c_str(),field.c_str(), increment);
}

bool RedisClient::hincrbyfloat(const string& key, const string& field, float increment, double& value)
{
	string strval = "";
	if(m_pRedisCmd->command_string(strval, "HINCRBYFLOAT %s %s %f", key.c_str(), field.c_str(), increment))
	{
		value = atof(strval.c_str());
		return true;
	}
	return false;
}

bool RedisClient::hkeys(const string& key, vector<string>& values)
{
    return m_pRedisCmd->command_list(values, "HKEYS %s", key.c_str());
}

bool RedisClient::hlen(const string& key, int64_t& count)
{
    return m_pRedisCmd->command_integer(count, "HLEN %s", key.c_str());
}

bool RedisClient::hmget( const string& key, const vector<string>& vField, ArrayReply& array)
{
	vector<string> vCmdData;
    vCmdData.push_back("HMGET");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vField);
    return m_pRedisCmd->commandargv_array(vCmdData, array);
}

bool RedisClient::hmset( const string& key, const vector<string>& vData)
{
	vector<string> vCmdData;
    vCmdData.push_back("HMSET");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vData);
    return m_pRedisCmd->commandargv_status(vCmdData);
}

bool RedisClient::hset(const string& key, const string& field, const string& value, int64_t& retval)
{
    return m_pRedisCmd->command_integer(retval, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool RedisClient::hsetnx(const string& key, const string& field, const string& value)
{
    return m_pRedisCmd->command_bool("HSETNX %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool RedisClient::hvals(const string& key, vector<string>& values)
{
    return m_pRedisCmd->command_list(values, "HVALS %s", key.c_str());
}


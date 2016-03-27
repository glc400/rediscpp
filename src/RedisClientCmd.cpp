#include "RedisClient.h"

bool RedisClient::config_get(const string& param, ArrayReply& arr_reply)
{
	return m_pRedisCmd->command_array(arr_reply, "CONFIG GET %s", param.c_str());
}

bool RedisClient::config_set(const string& param, const string& value)
{
	return m_pRedisCmd->command_bool("CONFIG SET %s %s", param.c_str(), value.c_str());
}

bool RedisClient::dbsize(int64_t& size)
{
	return m_pRedisCmd->command_integer(size, "DBSIZE");
}

bool RedisClient::flushall()
{
	return m_pRedisCmd->command_bool("FLUSHALL");
}

bool RedisClient::flushdb()
{
	return m_pRedisCmd->command_bool("FLUSHDB");
}

bool RedisClient::info(string& info)
{
	return m_pRedisCmd->command_string(info, "INFO");
}

bool RedisClient::lastsave(string& info, int64_t& timestamp)
{
	return m_pRedisCmd->command_integer(timestamp, "LASTSAVE");
}

bool RedisClient::save()
{
	return m_pRedisCmd->command_bool("SAVE");
}

bool RedisClient::time(string& timestamp)
{
	ArrayReply arraytime;
	if(m_pRedisCmd->command_array(arraytime, "TIME"))
	{
		timestamp = arraytime[0].str + '.' + arraytime[1].str;
		return true;
	}
	return false;
}

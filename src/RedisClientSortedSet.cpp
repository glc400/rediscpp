#include "RedisClient.h"

bool RedisClient::zadd(const string& key, const vector<string>& vValues, int64_t& count)
{
	vector<string> vCmdData;
    vCmdData.push_back("ZADD");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vValues);

    return m_pRedisCmd->commandargv_integer(vCmdData, count);
}

bool RedisClient::zscrad(const string& key, int64_t& count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zscrad");
        return false;
    }

    return m_pRedisCmd->command_integer(count, "ZSCRAD %s", key.c_str());
}

bool RedisClient::zcount(const string& key, string& min, string& max, int64_t& count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zcount");
        return false;
    }

    return m_pRedisCmd->command_integer(count, "ZCOUNT %s %s %s", key.c_str(), min.c_str(), max.c_str());
}

bool RedisClient::zincrby(const string& key, const double &increment, const string& member, string& value)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zincrby");
        return false;
    }

    return m_pRedisCmd->command_string(value, "ZINCRBY %s %f %s", key.c_str(), increment, member.c_str());
}

bool RedisClient::zrange(const string& key, int start, int end, vector<string>& vValues, bool withscore)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zincrby");
        return false;
    }

    if (withscore){
        return m_pRedisCmd->command_list(vValues, "ZRANGE %s %d %d %s", key.c_str(), start, end, "WITHSCORES");
    }
    return m_pRedisCmd->command_list(vValues, "ZRANGE %s %d %d", key.c_str(), start, end);
}

bool RedisClient::zrank(const string& key, const string& member, int64_t &rank)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zrank");
        return false;
    }

    return m_pRedisCmd->command_integer(rank, "ZRANK %s %s", key.c_str(), member.c_str());
}

bool RedisClient::zrem(const KEY& key, const vector<string>& vmembers, int64_t &count)
{
	vector<string> vCmdData;
    vCmdData.push_back("ZREM");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vmembers);

    return m_pRedisCmd->commandargv_integer(vCmdData, count);
}

bool RedisClient::zremrangebyrank(const string& key, int start, int stop, int64_t& count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zremrangebyrank");
        return false;
    }

    return m_pRedisCmd->command_integer(count, "ZREMRANGEBYRANK %s %d %d", key.c_str(), start, stop);
}

bool RedisClient::zrevrange(const string& key, int start, int end, vector<string>& vValues, bool withscore)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zrevrange");
        return false;
    }

    if (withscore){
        return m_pRedisCmd->command_list(vValues, "ZREVRANGE %s %d %d %s", key.c_str(), start, end, "WITHSCORES");
    }
    return m_pRedisCmd->command_list(vValues, "ZREVRANGE %s %d %d", key.c_str(), start, end);
}

bool RedisClient::zrevrank(const string& key, const string &member, int64_t& rank)
{
	if (0 == key.length()){
		m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zrevrank");
		return false;
	}

	return m_pRedisCmd->command_integer(rank, "ZREVRANK %s %s", key.c_str(), member.c_str());
}

bool RedisClient::zscore(const string& key, const string &member, string& score)
{
	if (0 == key.length()){
		m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "zscore");
		return false;
	}

	return m_pRedisCmd->command_string(score, "ZSCORE %s %s", key.c_str(), member.c_str());
}

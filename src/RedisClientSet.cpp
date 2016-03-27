#include "RedisClient.h"
#include <algorithm>


bool RedisClient::sadd(const string& key, const vector<string>& vValue, int64_t& count)
{
	vector<string> vCmdData;
    vCmdData.push_back("SADD");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vValue);

    return m_pRedisCmd->commandargv_integer(vCmdData, count);
}

bool RedisClient::scard(const string& key, int64_t& count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "scard");
        return false;
    }

    return m_pRedisCmd->command_integer(count, "SCARD %s", key.c_str());
}

bool RedisClient::sdiff(const vector<string>& vkey, vector<string>& sValue)
{
	size_t size = vkey.size();
    if (0 == size){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "sdiff");
        return false;
    }

    vector<string> *setData = new vector<string>[size];
    vector<string>::const_iterator iter_key = vkey.begin();

    int i = 0;
    for (; iter_key != vkey.end(); ++iter_key, ++i){
        const string &key = *iter_key;
        if (!smembers(key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n = 0;
    vector<string>::iterator endpos;
    while(n++<size-1) {
        endpos = set_difference(setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end(), sValue.begin());
        sValue.resize(endpos - sValue.begin());
    }

    delete [] setData;
    return true;
}

bool RedisClient::sdiffstore(const string& dstkey, const vector<string>& vkey, int64_t& count)
{
	vector<string> sValue;
    if (!sdiff(vkey, sValue))
    {
       return false;
    }

    return sadd(dstkey, sValue, count);
}

bool RedisClient::sinter(const vector<string>& vkey, vector<string>& sValue)
{
    size_t size = vkey.size();
    if (0 == size){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "sinter");
        return false;
    }

    vector<string> *setData = new vector<string>[size];
    vector<string>::const_iterator iter_key = vkey.begin();
    int i = 0;
    for (; iter_key != vkey.end(); ++iter_key, ++i){
        const string &key = *iter_key;
        if (!smembers(key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n = 0;
    vector<string>::iterator endpos;
    while(n++ < size-1){
        endpos = set_intersection(setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end(), sValue.begin());
        sValue.resize(endpos - sValue.begin());
    }

    delete [] setData;
    return true;
}

bool RedisClient::sinterstore(const string& destinationkey, const vector<string>& vkey, int64_t& count)
{
	vector<string> sValue;
    if (!sinter(vkey, sValue)) {
        return false;
    }
    return sadd(destinationkey, sValue, count);
}

bool RedisClient::sismember(const KEY& key, const string& member)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "sismember");
        return false;
    }

    return m_pRedisCmd->command_bool( "SISMEMBER %s %s", key.c_str(), member.c_str());
}

bool RedisClient::smembers(const string& key, vector<string>& vValue)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "smembers");
        return false;
    }

    return m_pRedisCmd->command_list(vValue, "SMEMBERS %s", key.c_str());
}

bool RedisClient::smove(const string& srckey, const string& deskey, const string& member)
{
    if (0 == srckey.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "smove");
        return false;
    }

    return m_pRedisCmd->command_bool("SMOVE %s", srckey.c_str(), deskey.c_str(), member.c_str());
}

bool RedisClient::spop(const KEY& key, string& member)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "spop");
        return false;
    }

    return m_pRedisCmd->command_string(member, "SPOP %s", key.c_str());
}

bool RedisClient::srandmember(const string& key, vector<string>& members, int count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "srandmember");
        return false;
    }

    if (0 == count){
        return m_pRedisCmd->command_list(members, "SRANDMEMBER %s", key.c_str());
    }

    return m_pRedisCmd->command_list( members, "SRANDMEMBER %s %d", key.c_str(), count);
}

bool RedisClient::srem(const KEY& key, const vector<string>& vmembers, int64_t& count)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "srem");
        return false;
    }

    vector<string> vCmdData;
    vCmdData.push_back("SREM");
    vCmdData.push_back(key);
    RedisCommand::AddParam(vCmdData, vmembers);

    return m_pRedisCmd->commandargv_integer(vCmdData, count);
}

bool RedisClient::sunion(const vector<string>& vkey, vector<string>& sValue)
{
    size_t size = vkey.size();
    if (0 == size){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "sunion");
        return false;
    }

    vector<string> *setData = new VALUES[size];
    vector<string>::const_iterator iter_key = vkey.begin();
    int i = 0;
    for (; iter_key != vkey.end(); ++iter_key, ++i) {
        const string &key = *iter_key;
        if (!smembers(key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n = 0;
    vector<string>::iterator endpos;
    while(n++ < size-1){
        endpos = set_union(setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end(), sValue.begin());
        sValue.resize(endpos - sValue.begin());
    }

    delete [] setData;
    return true;
}

bool RedisClient::sunionstore(const string& deskey, const vector<string>& vkey, int64_t& count)
{
	vector<string> sValue;
    if (!sunion(vkey, sValue)) {
        return false;
    }

    return sadd( deskey, sValue, count);
}

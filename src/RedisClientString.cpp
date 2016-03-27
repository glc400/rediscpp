#include "hiredis.h"
#include "RedisClient.h"

bool RedisClient::append(const string& key, const string& value)
{
    vector<string> vCmdData;
    vCmdData.push_back("APPEND");
    vCmdData.push_back(key);
    vCmdData.push_back(value);

    return m_pRedisCmd->commandargv_status(vCmdData);
}

bool RedisClient::bitcount(const string& key, int64_t bit, int start, int end)
{
    if (0 == key.length()){
        m_pRedisCmd->SetErrMsg(ERROR_ARGS_COMMAND, "bitcount");
        return false;
    }

    if (0 != start || 0 != end){
        return m_pRedisCmd->command_integer(bit, "BITCOUNT %s %d %d", key.c_str(), start, end);
    }

    return m_pRedisCmd->command_integer(bit, "BITCOUNT %s", key.c_str());
}

bool RedisClient::bitop(const EBitOP operation, const string& dstkey, const vector<string>& keys, int& length)
{
    int64_t intval = 0;
    static const char *op_cmd[4]= {"AND","OR","XOR","NOT"};

    vector<string> vCmdData;
    vCmdData.push_back("BITOP");
    vCmdData.push_back(op_cmd[operation]);
    vCmdData.push_back(dstkey);
    RedisCommand::AddParam(vCmdData, keys);

    bool bRet = m_pRedisCmd->commandargv_integer(vCmdData, intval);
    length = (int)intval;

    return bRet;
}

bool RedisClient::decr(const string& key, int64_t& result)
{
    return m_pRedisCmd->command_integer(result, "DECR %s", key.c_str());
}

bool RedisClient::decrby(const string& key, int byValue, int64_t& result)
{
    return m_pRedisCmd->command_integer(result, "DECRBY %s %d", key.c_str(), byValue);
}

bool RedisClient::get(const string& key, string& value)
{
    return m_pRedisCmd->command_string(value, "GET %s", key.c_str());
}

bool RedisClient::getbit(const string& key, int& offset, int& bit)
{
    int64_t intval = 0;
    bool bRet = m_pRedisCmd->command_integer(intval, "GETBIT %s %d", key.c_str(), offset);
    bit = (int)intval;
    return bRet;
}

bool RedisClient::getrange(const string& key, int start, int end, string& value)
{
	return m_pRedisCmd->command_string(value, "GETRANGE %s %d %d", key.c_str(), start, end);
}

bool RedisClient::getset(const string& key, const string& oldval, string& newval)
{
	return m_pRedisCmd->command_string(newval, "GETSET %s %s", key.c_str(), oldval.c_str());
}

bool RedisClient::incr(const string& key, int64_t& result)
{
    return m_pRedisCmd->command_integer(result, "INCR %s", key.c_str());
}

bool RedisClient::incrby(const string& key, int byValue, int64_t& result)
{
    return m_pRedisCmd->command_integer(result, "INCRBY %s %d", key.c_str(), byValue);
}

bool RedisClient::incrbyfloat(const string& key, double byVal, double& result)
{
	string retstr;
    bool bRet = m_pRedisCmd->command_string(retstr, "INCRBYFLOAT %s %f", key.c_str(), byVal);
    result = atof(retstr.c_str());
    return bRet;
}

bool RedisClient::mget(const vector<string>& keys, ReplyData& vData)
{
    bool bRet = false;

    vector<string>::const_iterator it = keys.begin();
    for (; it!=keys.end(); ++it){
        const string &key = *it;
        if (key.length() > 0)
        {
        	TReplyItem item;
            bool ret = m_pRedisCmd->command_string(item.str, "GET %s", key.c_str());
            if (!ret) {
                item.type = REDIS_REPLY_NIL;
                item.str  = "";
            } else {
                item.type = REDIS_REPLY_STRING;
                bRet = true;
            }
            vData.push_back(item);
        }
    }
    return bRet;
}

bool RedisClient::mget_test1(const vector<string>& keys, ReplyData& vData)
{
    return m_pRedisCmd->command_append("GET", keys, vData);
}

bool RedisClient::mget_test2(const vector<string>& keys, ReplyData& vData)
{
	vector<string> vCmdData;
    vCmdData.push_back("MGET");
    RedisCommand::AddParam(vCmdData, keys);
    return m_pRedisCmd->commandargv_array(vCmdData, vData);
}

bool RedisClient::mset(const vector<string>& vData)
{
	vector<string>::const_iterator it = vData.begin();
    while(it != vData.end())
    {
    	const string &key = *it++;
        const string &value = *it++;
        m_pRedisCmd->command_status("SET %s %s", key.c_str(), value.c_str());
    }
    return true;
}

bool RedisClient::mset(const map<string, string>& vData)
{
	map<string, string>::const_iterator it = vData.begin();
    while(it != vData.end())
    {
    	const string key = it->first;
        const string value = it->second;
        m_pRedisCmd->command_status("SET %s %s", key.c_str(), value.c_str());
    }
    return true;
}

bool RedisClient::msetnx(const vector<string>& vData)
{
	vector<string>::const_iterator it = vData.begin();
    while(it != vData.end())
    {
    	const string &key = *it++;
        const string &value = *it++;
        m_pRedisCmd->command_status("SETNX %s %s", key.c_str(), value.c_str());
    }
    return true;
}

bool RedisClient::psetex(const string& key, int milliseconds, const string& value)
{
    return m_pRedisCmd->command_bool("PSETEX %s %d %s", key.c_str(), milliseconds, value.c_str());
}

bool RedisClient::set(const string& key, const string& value, int len, int second)
{
	if(0 == len)
		len = value.length();

    if (0 == second){
        return m_pRedisCmd->command_bool("SET %s %b", key.c_str(), value.c_str(), len);
    } else {
        return m_pRedisCmd->command_bool("SET %s %b EX %d", key.c_str(), value.c_str(), len, second);
    }
}

bool RedisClient::setbit(const string& key, int offset, int64_t newval, int64_t oldval)
{
    return m_pRedisCmd->command_integer(oldval, "SETBIT %s %d %lld", key.c_str(), offset, newval);
}

bool RedisClient::setex(const string& key, int seconds, const string& value)
{
    vector<string> vCmdData;
    vCmdData.push_back("SETEX");
    vCmdData.push_back(key);
    vCmdData.push_back(toString(seconds));
    vCmdData.push_back(value);

    return m_pRedisCmd->commandargv_bool(vCmdData);
}

bool RedisClient::setnx(const string& key, const string& value)
{
	vector<string> vCmdData;
    vCmdData.push_back("SETNX");
    vCmdData.push_back(key);
    vCmdData.push_back(value);

    return m_pRedisCmd->commandargv_bool(vCmdData);
}

bool RedisClient::setrange(const string& key, int offset, const string& value, int& length)
{
    int64_t intval = 0;
    bool bRet = m_pRedisCmd->command_integer(intval, "SETRANGE %s %d %s", key.c_str(), offset, value.c_str());
    length = (int)intval;
    return bRet;
}

bool RedisClient::strlen(const string& key, int& length)
{
    int64_t intval = 0;
    bool bRet = m_pRedisCmd->command_integer(intval, "STRLEN %s", key.c_str());
    length = (int)intval;
    return bRet;
}

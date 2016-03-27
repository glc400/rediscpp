#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include "RedisCommand.h"

class RedisClient {
public:
	RedisClient(const std::string host = "localhost",
			unsigned int port = 6379,
			unsigned int db = 0,
			const std::string passwd = "",
			unsigned int timeout = 0,
			unsigned int minConn = 1,
			unsigned int maxConn = 2);
	RedisClient(RedisPool* pRedisPool);
	RedisClient(const std::string url);
	~RedisClient();
	RedisPool* GetRedisPool();

	RedisConnection* GetConnection();
	void FreeConnection(RedisConnection* pRedisConn);

	char* GetErrorInfo() { return RedisCommand::GetErrInfo(); }

	// --------------------[Server  ]-------------------- //
	bool config_get(const string& param, ArrayReply& arr_reply);
	bool config_set(const string& param, const string& value);
	bool dbsize(int64_t& size);
	bool flushall();
	bool flushdb();
	bool info(string& info);
	bool lastsave(string& info, int64_t& timestamp);
	bool save();
	bool time(string& timestamp);

	// --------------------[Key     ]-------------------- //
	bool del(const string& key);
	bool del(const vector<string>& vkey, int64_t& count);
	bool dump(const string& key, string& value);
	bool exists(const string& key);
	bool expire(const string& key, unsigned int second);
	bool expireat(const string& key, unsigned int timestamp);
	bool keys(const string& pattern, vector<string>& vResult);
	bool persist(const string& key);
	bool pexpire(const string& key, unsigned int milliseconds);
	bool pexpireat(const string& key, unsigned int millisecondstimestamp);
	bool pttl(const string& key, int64_t &milliseconds);
	bool randomkey(string& key);
	bool rename(const string& key, const string& newkey);
	bool renamenx(const string& key, const string& newkey);
	bool ttl(const string& key, int64_t &seconds);
	bool type(const string& key, string& retstr);


	// --------------------[String  ]-------------------- //
	bool append(const string& key, const string& value);
	bool bitcount(const string& key, int64_t bit, int start = 0, int end = 0);
	bool bitop(const EBitOP operation, const string& dstkey, const vector<string>& keys, int& length);
	bool decr(const string& key, int64_t& result);
	bool decrby(const string& key, int byValue, int64_t& result);
	bool get(const string& key, string& value);
	bool getbit(const string& key, int& offset, int& bit);
	bool getrange(const string& key, int start, int end, string& value);
	bool getset(const string& key, const string& newval, string& oldval);
	bool incr(const string& key, int64_t& result);
	bool incrby(const string& key, int byValue, int64_t& result);
	bool incrbyfloat(const string& key, double byVal, double& result);
	bool mget(const vector<string>& keys, ReplyData& vData);
	bool mget_test1(const vector<string>& keys, ReplyData& vData);
	bool mget_test2(const vector<string>& keys, ReplyData& vData);
	bool mset(const vector<string>& vData);
	bool mset(const map<string, string>& vData);
	bool msetnx(const vector<string>& vData);
	bool psetex(const string& key, int milliseconds, const string& value);
	bool set(const string& key, const string& value, int len = 0, int second = 0);
	bool setbit(const string& key, int offset, int64_t newval, int64_t oldval);
	bool setex(const string& key, int seconds, const string& value);
	bool setnx(const string& key, const string& value);
	bool setrange(const string& key, int offset, const string& value, int& length);
	bool strlen(const string& key, int& length);

	// --------------------[Hash    ]-------------------- //
	bool hdel(const string& key, const string& field, int64_t& count);
	bool hdel(const string& key, const vector<string>& vfiled, int64_t& count);
	bool hexist(const string& key, const string& field);
	bool hget(const string& key, const string& field, string& value);
	bool hgetall( const string& key, ArrayReply& array);
	bool hincrby(const string& key, const string& field, int64_t increment, int64_t& num );
	bool hincrbyfloat(const string& key, const string& field, float increment, double& value);
	bool hkeys(const string& key, vector<string>& values);
	bool hlen(const string& key, int64_t& count);
	bool hmget( const string& key, const vector<string>& vField, ArrayReply& array);
	bool hmset( const string& key, const vector<string>& vData);
	bool hset(const string& key, const string& field, const string& value, int64_t& retval);
	bool hsetnx(const string& key, const string& field, const string& value);
	bool hvals(const string& key, vector<string>& values);

	// --------------------[List    ]-------------------- //
	bool lindex(const string& key, int64_t index, string& value);
	bool linsert(const string& key, const ELIST_MODE mod, const string& pivot, const string& value, int64_t& retval);
	bool llen(const string& key, int64_t& retval);
	bool lpop(const string& key, string& value);
	bool lpush(const string& key, const vector<string>& vValue, int64_t& length);
	bool lpushx(const string& key, string& value, int64_t& length);
	bool lrange(const string& key, int64_t start, int64_t end, ArrayReply& array);
	bool lrem(const string& key, int count, const string& value, int64_t num);
	bool lset(const string& key, int index, const string& value);
	bool ltrim(const string& key, int start, int end);
	bool rpop(const string& key, string& value);
	bool rpoplpush(const string& key_src, const string& key_dest, string& value);
	bool rpush(const string& key, const vector<string>& vValue, int64_t& length);
	bool rpushx(const string& key, const string& value, int64_t& length);

	// --------------------[Set    ]-------------------- //
	bool sadd(const string& key, const vector<string>& vValue, int64_t& count);
	bool scard(const string& key, int64_t& count);
	bool sdiff(const vector<string>& vkey, vector<string>& sValue);
	bool sdiffstore(const string& dstkey, const vector<string>& vkey, int64_t& count);
	bool sinter(const vector<string>& vkey, vector<string>& sValue);
	bool sinterstore(const string& destinationkey, const vector<string>& vkey, int64_t& count);
	bool sismember(const KEY& key, const string& member);
	bool smembers(const string& key, vector<string>& vValue);
	bool smove(const string& srckey, const string& deskey, const string& member);
	bool spop(const KEY& key, string& member);
	bool srandmember(const string& key, vector<string>& members, int count);
	bool srem(const KEY& key, const vector<string>& vmembers, int64_t& count);
	bool sunion(const vector<string>& vkey, vector<string>& sValue);
	bool sunionstore(const string& deskey, const vector<string>& vkey, int64_t& count);

	// --------------------[SortedSet]------------------- //
	bool zadd(const string& key, const vector<string>& vValues, int64_t& count);
	bool zscrad(const string& key, int64_t& count);
	bool zcount(const string& key, string& min, string& max, int64_t& count);
	bool zincrby(const string& key, const double &increment, const string& member, string& value);
	bool zrange(const string& key, int start, int end, vector<string>& vValues, bool withscore);
	bool zrank(const string& key, const string& member, int64_t &rank);
	bool zrem(const KEY& key, const vector<string>& vmembers, int64_t &count);
	bool zremrangebyrank(const string& key, int start, int stop, int64_t& count);
	bool zrevrange(const string& key, int start, int end, vector<string>& vValues, bool withscore);
	bool zrevrank(const string& key, const string &member, int64_t& rank);
	bool zscore(const string& key, const string &member, string& score);

private:
	RedisPool* m_pRedisPool;
	RedisCommand* m_pRedisCmd;
};

#endif // REDIS_CLIENT_H

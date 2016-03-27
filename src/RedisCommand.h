#ifndef REDIS_COMMAND_H
#define REDIS_COMMAND_H

#include "hiredis.h"
#include "RedisPool.h"


class RedisCommand
{
public:
	RedisCommand(RedisPool* pRedisPool);
	~RedisCommand();

	static bool SetErrInfo(const char* info, int len);
	static bool SetErrInfo(void *p);
	void SetErrMsg(const char* fmt, ...);
	static char* GetErrInfo() { return m_strError; }

	redisReply* command(const char* cmd);
	bool command_bool(const char *cmd, ...);
	static bool command_bool(redisContext* pRedisCtx, const char *cmd, ...);
	static bool command_string(redisContext* pRedisCtx, string& retstr, const char *cmd, ...);
	bool command_status(const char* cmd, ...);
	bool command_integer(int64_t &retval, const char* cmd, ...);
	bool command_string(string& retstr, const char* cmd, ...);
	bool command_list(VDATA& vData, const char* cmd, ...);
	bool command_array(ArrayReply& array, const char* cmd, ...);
	bool commandargv_bool(VDATA& vData);
	bool commandargv_status(const VDATA& vData);
	bool commandargv_array(const VDATA& vDataIn, ArrayReply& array);
	bool commandargv_array(const VDATA& vDataIn, VALUES& array);
	bool commandargv_integer(const VDATA& vDataIn, int64_t& retval);
	bool command_append(const char* cmd, const vector<string>& keys, vector<TReplyItem>& vReply);

	static void AddParam(vector<string>& vDes, const vector<string>& vSrc){
		vDes.insert(vDes.end(), vSrc.begin(), vSrc.end());
	}
private:
	RedisPool* m_pRedisPool;
	static char m_strError[LEN_ERROR_INFO+1];
};


#endif /* REDIS_COMMAND_H */

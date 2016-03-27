#ifndef REDISDEFINE_H_
#define REDISDEFINE_H_

#include <list>
#include <map>
#include <new>
#include <vector>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <sstream>
#include <functional>
#include <iostream>

#define THREAD_STACK_SIZE 10*1024*1024   //10M
#define LEN_ERROR_INFO 1024
#define LEN_REPLAY_ITEM 4096
#define SLEEP_SECONDS 1

#define ERROR_GET_CONNECT       "Get redis connection error"
#define ERROR_CONNECT_CLOSED    "Redis connection be closed"
#define ERROR_NULL_CONTEXT      "Null RedisContext"
#define ERROR_ARGS_COMMAND      "ERR arguments for '%s' command"
#define ERROR_APPEND_COMMAND    "ERR "


typedef struct _TReplyItem{
    int type;
    std::string str;

    _TReplyItem& operator=(const _TReplyItem&data){
        type = data.type;
        str  = data.str;
        return *this;
    }
} TReplyItem;

typedef enum _ESortOrder
{
    ASC = 0,
    DESC = 1
} ESortOrder;

typedef enum _EBitOP
{
    AND = 0,
    OR  = 1,
    XOR = 2,
    NOT = 3
} EBitOP;

typedef struct _TSortLimit
{
	int offset;
	int count;
} TSortLimit;

typedef enum _ELIST_MODE{
    BEFORE = 0,
    AFTER  = 1
} ELIST_MODE;

typedef std::map<std::string, std::string> MapReply;
typedef std::vector<TReplyItem> ArrayReply;
typedef std::vector<TReplyItem> ReplyData;

typedef std::string KEY;
typedef std::string VALUE;
typedef std::vector<KEY> KEYS;
typedef std::vector<VALUE> VALUES;
typedef std::vector<std::string> VDATA;

template<class T>
static std::string toString(const T &t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template<class Type>
static Type fromString(const std::string& str) {
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

static bool inString(const std::string& str, const std::string& fromstr){
	std::istringstream istr(fromstr);
	std::string tmp;
	while(istr >> tmp){
		if(tmp == str) return true;
	}
	return false;
}


#endif /* REDISDEFINE_H_ */

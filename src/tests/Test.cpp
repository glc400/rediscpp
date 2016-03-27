#include "RedisClient.h"


int main()
{
	RedisClient client("127.0.0.1", 6880, 0, "", 0, 1, 5);
	RedisConnection* pRedisConn = client.GetConnection();
	if(NULL == pRedisConn)
	{
		cout << "Get connection error!" << endl;
		cout << client.GetErrorInfo() << endl;
		return -1;
	}

	ArrayReply replay_data;
	map<string, string> mData;
	mData.insert(pair<string, string>("Name1", "GLC1"));
	mData.insert(pair<string, string>("Name2", "GLC2"));
	mData.insert(pair<string, string>("Name3", "GLC3"));
	client.mset(mData);

	return 0;
}

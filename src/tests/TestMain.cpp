#include "TestRedisConn.h"
#include "TestRedisKey.h"
#include "TestRedisString.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}

# ������������
CC=xlC_r
CFLAGS=-q64 -g
CXXFLAGS=-q64 -g
LDFLAGS=-qmkshrobj -bnoentry -brtl -berok -bh:5
AR=ar
AR_FLAGS=-X64 rcs

# Դ����Ŀ¼�����
VPATH=.:../src:../src/tests

# ������ͷ�ļ������
INCLUDE=-I../src/ -I$(HOME)/local/include

# �����ⶨ���
LD_LIBS=-L$(HOME)/local/lib -lhiredis -lpthread -lgtest

# OBJ�����
OBJS=RedisClient.o RedisConnection.o RedisPool.o RedisException.o \
RedisCommand.o RedisClientCmd.o RedisClientKey.o RedisClientString.o \
RedisClientHash.o RedisClientList.o RedisClientSet.o RedisClientSortedSet.o
OBJS_TEST=Test.o $(OBJS)
OBJS_TEST_SINGLE=TestSingle.o
OBJS_GTEST_CONN=TestMain.o $(OBJS)

# ����ﶨ���
TARGET_ST=libredisclient.a
TARGET_DY=libredisclient.so
TARGET_TEST=test_redis
TARGET_TEST_SINGLE=test_single
TARGET_GTEST_CONN=gtest_redis

.SUFFIXES: .cpp .c .h
.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
.c.o:
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# ALL�����
all: $(TARGET_ST) $(TARGET_DY)
$(TARGET_ST): $(OBJS)
	$(AR) $(AR_FLAGS) $(TARGET_ST) $(OBJS)
$(TARGET_DY): $(OBJS)
	$(CC) -G $(LDFLAGS) -o $(TARGET_DY) $(LD_LIBS)

test: $(TARGET_TEST) $(TARGET_TEST_SINGLE) $(TARGET_GTEST_CONN)
$(TARGET_TEST): $(OBJS_TEST)
	$(CC) -o $(TARGET_TEST) $(OBJS_TEST) $(LD_LIBS)
$(TARGET_TEST_SINGLE): $(OBJS_TEST_SINGLE)
	$(CC) -o $(TARGET_TEST_SINGLE) $(OBJS_TEST_SINGLE) $(LD_LIBS)
$(TARGET_GTEST_CONN): $(OBJS_GTEST_CONN)
	$(CC) -o $(TARGET_GTEST_CONN) $(OBJS_GTEST_CONN) $(LD_LIBS)

# clean�����
clean:
	rm -f *.o test_*
	rm -f $(TARGET_ST) $(TARGET_DY)
	rm -f $(TARGET_TEST) $(TARGET_TEST_SINGLE) $(TARGET_GTEST_CONN)

.PHONY: all clean
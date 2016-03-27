#ifndef REDIS_SYNCH_H
#define REDIS_SYNCH_H

#include <assert.h>
#include <pthread.h>
#include <errno.h>


class MutexLock
{
public:
    MutexLock()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        int ret = pthread_mutex_init(&m_pMutex, &attr);
        assert(ret == 0); (void) ret;
    }

    ~MutexLock()
    {
        int ret = pthread_mutex_destroy(&m_pMutex);
        assert(ret == 0); (void) ret;
    }

    void Lock()
    {
        pthread_mutex_lock(&m_pMutex);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&m_pMutex);
    }

    pthread_mutex_t* GetPthreadMutex() /* non-const */
    {
        return &m_pMutex;
    }

private:
    pthread_mutex_t m_pMutex;
};

class MutexLockGuard
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
        : m_cMutex(mutex)
    {
    	m_cMutex.Lock();
    }

    ~MutexLockGuard()
    {
    	m_cMutex.Unlock();
    }

private:
    MutexLock& m_cMutex;
};

class Condition 
{
public:
    explicit Condition(MutexLock& mutex)
        : m_cMutex(mutex)
    {
        pthread_cond_init(&m_pCond, NULL);
    }

    ~Condition()
    {
        pthread_cond_destroy(&m_pCond);
    }

    void Wait()
    {
        pthread_cond_wait(&m_pCond, m_cMutex.GetPthreadMutex());
    }

    bool WaitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += seconds;
        return ETIMEDOUT == pthread_cond_timedwait(&m_pCond, m_cMutex.GetPthreadMutex(), &abstime);
    }

    void Notify()
    {
        pthread_cond_signal(&m_pCond);
    }

    void NotifyAll()
    {
        pthread_cond_broadcast(&m_pCond);
    }

private:
    MutexLock& m_cMutex;
    pthread_cond_t m_pCond;
};


#endif // REDIS_SYNCH_H

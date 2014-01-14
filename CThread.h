/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef _myThread_CTHREAD_H_
#define _myThread_CTHREAD_H_
#include <pthread.h>

enum threadType_e {
    JOIN_E,
    DETACH_E
};

enum mutextAttr_e {
    MUTEX_NORMAL_E,
    MUTEX_ERROR_CHECK_E,
    MUTEX_RECURSIVE_E
};

class CThread
{
private:
    pthread_t       m_tid;
    threadType_e    m_threadType;

private:
    static void _cleanupHandler(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);
        _obj->cleanuUp();
    }

    static void* _threadFunc(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);

        pthread_cleanup_push(CThread::_cleanupHandler, obj);
        _obj->onStartup();
        _obj->process();
        _obj->onExit();
        pthread_cleanup_pop(1);

        return NULL;
    }

public:
    CThread(threadType_e threadType = DETACH_E) {};

    virtual ~CThread()
    {
        pthread_cancel(getThreadId());
    }

    int start(void)
    {
        int rtn;

        rtn = pthread_create(&m_tid, NULL, CThread::_threadFunc, this);
        if (!rtn && (m_threadType == DETACH_E)) pthread_detach(m_tid);

        return rtn;
    }

    /* 
     *  thread body 
     */
    virtual void process(void) = 0;
    virtual void onStartup(void) {}
    virtual void onExit(void) {}
    virtual void cleanuUp(void) {}

    pthread_t getThreadId(void)
    {
        return m_tid;
    }

    threadType_e getThreadType(void)
    {
        return m_threadType;
    }

    int join(void **threadReturn = NULL)
    {
        int rtn = -1;

        if (!pthread_equal(pthread_self(), m_tid)) {
            rtn = pthread_join(m_tid, threadReturn);
        }
        return rtn;
    }

    static void mutexInit(pthread_mutex_t& mtx, mutextAttr_e mutexAttr = MUTEX_NORMAL_E);

    static void mutexDestroy(pthread_mutex_t& mtx)
    {
        pthread_mutex_destroy(&mtx);
    }

    static void condInit(pthread_cond_t& cond)
    {
        pthread_condattr_t condattr;

        pthread_condattr_init(&condattr);
        pthread_cond_init(&cond, &condattr);
        pthread_condattr_destroy(&condattr);
    }

    static void lock(pthread_mutex_t& mtx)
    {
        pthread_mutex_lock(&mtx);
    }

    static void tryLock(pthread_mutex_t& mtx)
    {
        pthread_mutex_trylock(&mtx);
    }

    static void unlock(pthread_mutex_t& mtx)
    {
        pthread_mutex_unlock(&mtx);
    }

    static void condWait(pthread_cond_t& cond, pthread_mutex_t& mtx)
    {
        pthread_cond_wait(&cond, &mtx);
    }

    static void condTimedWait(pthread_cond_t& cond, pthread_mutex_t& mtx, const struct timespec& abstime)
    {
        pthread_cond_timedwait(&cond, &mtx, &abstime);
    }

    static void condSignal(pthread_cond_t& cond)
    {
        pthread_cond_signal(&cond);
    }

    static void condBroadcast(pthread_cond_t& cond)
    {
        pthread_cond_broadcast(&cond);
    }

};


#endif


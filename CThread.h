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
        _obj->_cleanup();
    }

    static void* _threadFunc(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);

        pthread_cleanup_push(CThread::_cleanupHandler, obj);
        _obj->doWork();
        pthread_cleanup_pop(1);

        return NULL;
    }


protected:
    virtual void _cleanup(void) { }

public:
    CThread(threadType_e threadType = DETACH_E);

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
    virtual void doWork(void) = 0;

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

    void mutexInit(pthread_mutex_t& mtx, mutextAttr_e mutexAttr = MUTEX_NORMAL_E);

    void condInit(pthread_cond_t& cond)
    {
        pthread_condattr_t condattr;

        pthread_condattr_init(&condattr);
        pthread_cond_init(&cond, &condattr);
        pthread_condattr_destroy(&condattr);
    }

    void lock(pthread_mutex_t& mtx)
    {
        pthread_mutex_lock(&mtx);
    }

    void tryLock(pthread_mutex_t& mtx)
    {
        pthread_mutex_trylock(&mtx);
    }

    void unlock(pthread_mutex_t& mtx)
    {
        pthread_mutex_unlock(&mtx);
    }

    void condWait(pthread_cond_t& cond, pthread_mutex_t& mtx)
    {
        pthread_cond_wait(&cond, &mtx);
    }

    void condTimedWait(pthread_cond_t& cond, pthread_mutex_t& mtx, const struct timespec& abstime)
    {
        pthread_cond_timedwait(&cond, &mtx, &abstime);
    }

    void condSignal(pthread_cond_t& cond)
    {
        pthread_cond_signal(&cond);
    }

    void condBroadcast(pthread_cond_t& cond)
    {
        pthread_cond_broadcast(&cond);
    }

};


#endif


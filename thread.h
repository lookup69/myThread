/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef _LOOKUP69_THREAD_H_
#define _LOOKUP69_THREAD_H_
#include <pthread.h>

namespace lkup69 {
    enum threadType_e {
        THREAD_JOIN_E,
        THREAD_DETACH_E
    };

    enum mutextAttr_e {
        THREAD_MUTEX_NORMAL_E,
        THREAD_MUTEX_ERROR_CHECK_E,
        THREAD_MUTEX_RECURSIVE_E
    };

    class Thread
    {
    private:
        pthread_t       m_tid;
        threadType_e    m_threadType;
        pthread_mutex_t m_selfMutx;

    private:
        static void *threadFunc_(void *obj)
        {
            Thread *obj_;
            void   *ret;

            obj_ = reinterpret_cast<Thread *>(obj);
            obj_->onStartup();
            ret = obj_->process();
            obj_->onExit();

            return ret;
        }

    public:
        Thread(threadType_e threadType = THREAD_DETACH_E) : m_threadType(threadType) { };

        virtual ~Thread()
        {
            pthread_cancel(getThreadID());
        }

        /*
         *  mutex operation
         */
        static int mutexInit(pthread_mutex_t &mtx, mutextAttr_e mutexAttr = THREAD_MUTEX_NORMAL_E);

        static int mutexDestroy(pthread_mutex_t &mtx)
        {
            return pthread_mutex_destroy(&mtx);
        }

        static int lock(pthread_mutex_t &mtx)
        {
            //printf("pthread_mutex_lock(%p)\n", &mtx);
            return pthread_mutex_lock(&mtx);
        }

        static int tryLock(pthread_mutex_t &mtx)
        {
            return pthread_mutex_trylock(&mtx);
        }

        static int unlock(pthread_mutex_t &mtx)
        {
            //printf("pthread_mutex_unlock(%p)\n", &mtx);
            return pthread_mutex_unlock(&mtx);
        }

        /*
         *  read write lock
         */
        static int rwLockInit(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_init(&rwlock, NULL);
        }

        static int rwLockDestroy(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_destroy(&rwlock);
        }

        static int readLock(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_rdlock(&rwlock);
        }

        static int readTryLock(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_tryrdlock(&rwlock);
        }

        static int writeLock(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_wrlock(&rwlock);
        }

        static int writeTryLock(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_trywrlock(&rwlock);
        }

        static int readWriteUnlock(pthread_rwlock_t &rwlock)
        {
            return pthread_rwlock_unlock(&rwlock);
        }

        /*
         *  condition operation
         */
        static int condInit(pthread_cond_t &cond)
        {
            return pthread_cond_init(&cond, NULL);
        }

        static int condDestroy(pthread_cond_t &cond)
        {
            return pthread_cond_destroy(&cond);
        }

        static int condWait(pthread_cond_t &cond, pthread_mutex_t &mtx)
        {
            return pthread_cond_wait(&cond, &mtx);
        }

        static int condTimedWait(pthread_cond_t &cond, pthread_mutex_t &mtx, const struct timespec &abstime)
        {
            return pthread_cond_timedwait(&cond, &mtx, &abstime);
        }

        static int condSignal(pthread_cond_t &cond)
        {
            return pthread_cond_signal(&cond);
        }

        static int condBroadcast(pthread_cond_t &cond)
        {
            return pthread_cond_broadcast(&cond);
        }

        /*
         *  create thread
         */
        int start(void)
        {
            int rtn;

            rtn = pthread_create(&m_tid, NULL, Thread::threadFunc_, this);
            if(!rtn && (m_threadType == THREAD_DETACH_E))
                pthread_detach(m_tid);

            return rtn;
        }

        /*
         *
         */
        pthread_t getThreadID(void)
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

            if(!pthread_equal(pthread_self(), m_tid)) {
                rtn = pthread_join(m_tid, threadReturn);
            }
            return rtn;
        }

        int selfLock(void)
        {
            return pthread_mutex_lock(&m_selfMutx);
        }

        int selfUnLock(void)
        {
            return pthread_mutex_unlock(&m_selfMutx);
        }
       
        /* 
         *  thread body 
         */
        virtual void *process(void) = 0;

        /*
         *  initialize extra resources
         */
        virtual void onStartup(void) { }

        /*
         *  release extra resources
         */
        virtual void onExit(void) { }
    };
}
#endif


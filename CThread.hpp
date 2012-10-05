#ifndef _myThread_CTHREAD_HPP
#define _myThread_CTHREAD_HPP
#include <pthread.h>

enum deatachsatate_e {
    JOINABLE,
    DEATACHED
};

class CThread {
private:
    pthread_t           thread_id;
    int             detachstate;
    bool            execState;
    bool            bExit;
    bool            isCancelable;

private:

    static void _cleanupHandler(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);
        _obj->setbExit();
    }

    static void *thread_function(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);

        pthread_cleanup_push(CThread::_cleanupHandler, obj);
        _obj->do_work();
        pthread_cleanup_pop(1);

        return NULL;
    }

    static void threadExitIndicate(void *obj)
    {
        CThread *_obj;
        _obj = reinterpret_cast<CThread *>(obj);
        _obj->setbExit();

    }

public:
    CThread(deatachsatate_e attribute = JOINABLE) ;
    virtual ~CThread() 
    {
        if(!bExit) {
            pthread_cancel(getThreadId());
        }
    }

    int start(void)
    {
        int rtn;

        rtn = pthread_create(&thread_id,NULL,CThread::thread_function,this);

        return rtn;
    }

    virtual void do_work(void){};



    pthread_t getThreadId(void)
    {
        return thread_id;
    }

    int join(void **thread_return = NULL)
    {
        int rtn = -1;

        if(!pthread_equal(pthread_self(),thread_id)) {
            rtn = pthread_join(thread_id,thread_return);
        }
        return rtn;
    }

    bool isDetached(void)
    {
        if(detachstate == PTHREAD_CREATE_JOINABLE)
            return false;

        return true;
    }

    void setbExit(void)
    {
        bExit = true;
    }             

};


#endif


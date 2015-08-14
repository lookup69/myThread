/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#include "thread.h"
using namespace lookup69;

int Thread::mutexInit(pthread_mutex_t &mtx, mutextAttr_e mutexAttr)
{
    pthread_mutexattr_t mtxAttr;
    int ret;

    if((ret = pthread_mutexattr_init(&mtxAttr)) < 0)
        return ret;

    if (mutexAttr == THREAD_MUTEX_ERROR_CHECK_E) {
        if((ret = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK)) < 0)
            goto err_;
    } else if (mutexAttr == THREAD_MUTEX_RECURSIVE_E) {
        if((ret = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_RECURSIVE)) < 0)
            goto err_;
    } else {
        if((ret = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_NORMAL)) < 0)
            goto err_;
    }

    pthread_mutex_init(&mtx, &mtxAttr);
    pthread_mutexattr_destroy(&mtxAttr);

err_:
    return ret;
}



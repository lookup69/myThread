/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#include "CThread.h"

void CThread::mutexInit(pthread_mutex_t &mtx, mutextAttr_e mutexAttr)
{
    pthread_mutexattr_t mtxAttr;

    pthread_mutexattr_init(&mtxAttr);
    if (mutexAttr == MUTEX_ERROR_CHECK_E) 
        pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK);
    if (mutexAttr == MUTEX_RECURSIVE_E) 
        pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_RECURSIVE);
    else 
        pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_NORMAL);

    pthread_mutex_init(&mtx, &mtxAttr);
    pthread_mutexattr_destroy(&mtxAttr);
}



/*
*                    Copyright (c), NXP Semiconductors
*
*                       (C)NXP Semiconductors B.V.2014
*         All rights are reserved. Reproduction in whole or in part is
*        prohibited without the written consent of the copyright owner.
*    NXP reserves the right to make changes without notice at any time.
*   NXP makes no warranty, expressed, implied or statutory, including but
*   not limited to any implied warranty of merchantability or fitness for any
*  particular purpose, or that the use will not infringe any third party patent,
*   copyright or trademark. NXP must not be liable for any loss or damage
*                            arising from its use.
*
*/
#include <cutils/log.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "phDTAOSAL.h"

/*
****************************** Macro Definitions ******************************
*/
#define LPVOID void*
#define LOG_FUNCTION_ENTRY phOsal_LogDebugString((const uint8_t*)"Osal>:Entered:",(const uint8_t*)__FUNCTION__)
#define LOG_FUNCTION_EXIT  phOsal_LogDebugString((const uint8_t*)"Osal>:Exit:",(const uint8_t*)__FUNCTION__)

/*
*************************** Function Definitions ******************************
*/

OSALSTATUS phOsal_ThreadCreate   (void                         **hThread,
                                 pphOsal_ThreadFunction_t   pThreadFunction,
                                 void                          *pParam)
{
    int32_t status = 0;
    LOG_FUNCTION_ENTRY;
    if( (NULL == hThread) || (NULL == pThreadFunction) )
    {
        return OSALSTATUS_INVALID_PARAMS;
    }

    /* Check for successfull creation of thread */
    status = pthread_create((pthread_t *)hThread,
                            NULL,
                            (void *)pThreadFunction,
                            pParam);
    if(0 != status)
    {
     phOsal_LogError((const uint8_t*)"Osal>Unable to create Thread");
     return OSALSTATUS_FAILED;
    }

    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

uint32_t phOsal_ThreadGetTaskId(void)
{
    uint32_t dwThreadId = 0;
    LOG_FUNCTION_ENTRY;
    LOG_FUNCTION_EXIT;
    return dwThreadId;
}

OSALSTATUS phOsal_ThreadDelete(void *hThread)
{
    void *pRetVal;
    uint32_t status=0;
    LOG_FUNCTION_ENTRY;
    if(NULL == hThread)
    {
        return OSALSTATUS_INVALID_PARAMS;
    }
    status = pthread_join((pthread_t)hThread,
                                    &pRetVal);
    if(0 != status)
    {
        phOsal_LogError((const uint8_t*)"Osal>Unable to delete Thread");
        return OSALSTATUS_FAILED;
    }
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

#ifdef ENABLE_ADVANCED_FUNCS
OSALSTATUS phOsal_ThreadSuspend(void *hThread)
{
    OSALSTATUS wSuspendStatus = OSALSTATUS_SUCCESS;
    LOG_FUNCTION_ENTRY;
    LOG_FUNCTION_EXIT;
    return wSuspendStatus;
}

OSALSTATUS phOsal_ThreadWakeUp(void *hThread)
{
    OSALSTATUS wResumeStatus = OSALSTATUS_SUCCESS;
    LOG_FUNCTION_ENTRY;
    LOG_FUNCTION_EXIT;
    return wResumeStatus;
}
#endif


OSALSTATUS phOsal_ThreadSetPriority(void *hThread,int32_t sdwPriority)
{
    uint32_t dwStatus=0;
    struct sched_param param;
    int32_t policy;
    LOG_FUNCTION_ENTRY;
    if(NULL == hThread)
    {
        return OSALSTATUS_INVALID_PARAMS;
    }
    dwStatus = pthread_getschedparam((pthread_t)hThread, &policy, &param);
   if(dwStatus != 0)
    {
        phOsal_LogErrorU32h((const uint8_t*)"Osal>Unable to get thread params.Error=",(uint32_t)dwStatus);
        phOsal_LogErrorString((const uint8_t*)"Osal>",(const uint8_t*)__FUNCTION__);
        return OSALSTATUS_FAILED;
    }
    param.sched_priority = sdwPriority;
    dwStatus = pthread_setschedparam((pthread_t)hThread, policy, &param);
    if(dwStatus != 0)
    {
        phOsal_LogErrorU32h((const uint8_t*)"Osal>Unable to Set thread Priority.Error=",(uint32_t)dwStatus);
        phOsal_LogErrorString((const uint8_t*)"Osal>",(const uint8_t*)__FUNCTION__);
        return OSALSTATUS_FAILED;
    }
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

static void * phOsal_ThreadProcedure(void *lpParameter)
{
    return lpParameter;
}

OSALSTATUS phOsal_SemaphoreCreate(void        **hSemaphore,
                                 uint8_t     bInitialValue,
                                 uint8_t     bMaxValue)
{
    int32_t status=0;
    LOG_FUNCTION_ENTRY;
    phOsal_LogInfoU32d((const uint8_t*)"Osal>Sem Max Value:",(uint32_t)bMaxValue);

    if(hSemaphore == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Semaphore Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    *hSemaphore = malloc(sizeof(sem_t));
    if(*hSemaphore == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Unable to allocate memory for semaphore");
        return OSALSTATUS_FAILED;
    }

    status = sem_init(*hSemaphore, 0, bInitialValue);
    if(status == -1)
    {
        phOsal_LogErrorU32d((const uint8_t*)"Osal>Unable to allocate memory for semaphore.Status=",(uint32_t)status);
        return OSALSTATUS_FAILED;
    }
    phOsal_LogInfo((const uint8_t*)"Osal> Semaphore Created");
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}


OSALSTATUS phOsal_SemaphorePost(void *hSemaphore)
{
    int32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hSemaphore == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Semaphore Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(sem_getvalue(hSemaphore,&checkval) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal> Semaphore Not available");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(sem_post(hSemaphore) == -1)
   {
       phOsal_LogError((const uint8_t*)"Osal> error in sem Post");
       return OSALSTATUS_INVALID_PARAMS;
   }

    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_SemaphoreWait(void *hSemaphore,
                                uint32_t timeout_ms)
{
    int32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hSemaphore == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Semaphore Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(sem_getvalue(hSemaphore,&checkval) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal> Semaphore Not available");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(timeout_ms == 0)
    {
        if(sem_wait(hSemaphore) == -1)
        {
             phOsal_LogError((const uint8_t*)"Osal> Error in Semaphore infinite wait !!");
             return OSALSTATUS_INVALID_PARAMS;
        }
    }
    else
    {
        struct timespec xtms;
        int32_t status=0;
        if(clock_gettime(CLOCK_REALTIME, &xtms) == -1)
        {
             phOsal_LogError((const uint8_t*)"Osal> Error in Getting current CPU time!!");
             return OSALSTATUS_INVALID_PARAMS;
        }

        /*Extract seconds and nanoseconds information from time in milliseconds*/
        xtms.tv_sec  += (time_t)timeout_ms/1000;
        xtms.tv_nsec += ((long)(timeout_ms%1000))*(1000000);

        while ((status = sem_timedwait(hSemaphore, &xtms)) == -1 && errno == EINTR)
        {
             phOsal_LogError((const uint8_t*)"Osal>Error in sem_timedwait restart it!!");
             continue;       /* Restart if interrupted by handler */
        }
        /* Check what happened */
        if (status == -1)


        {
             if (errno == ETIMEDOUT)
             {
                 phOsal_LogError((const uint8_t*)"Osal>sem_timedwait() timed out");
                 return OSALSTATUS_SEM_TIMEOUT;
             }
             else
             {
                 phOsal_LogError((const uint8_t*)"Osal>sem_timedwait");
                 return OSALSTATUS_FAILED;
             }
        }
        else
        {
             phOsal_LogInfo((const uint8_t*)"Osal>sem_timedwait() succeeded");
        }
    }
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_SemaphoreDelete(void *hSemaphore)
{
    int32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hSemaphore == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Semaphore Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(sem_getvalue(hSemaphore,&checkval) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal> Semaphore Not available");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(sem_destroy(hSemaphore) == -1)
    {
       phOsal_LogError((const uint8_t*)"Osal> Semaphore Destroy Failed");
       return OSALSTATUS_FAILED;
    }

    free(hSemaphore);
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_MutexCreate(void        **hMutex)
{
    int32_t status=0;
    LOG_FUNCTION_ENTRY;

    if(hMutex == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Mutex Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    *hMutex = malloc(sizeof(pthread_mutex_t));
    if(*hMutex == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Unable to allocate memory for mutex");
        return OSALSTATUS_FAILED;
    }

    status = pthread_mutex_init(*hMutex, 0);
    if(status == -1)
    {
        phOsal_LogErrorU32d((const uint8_t*)"Osal>Error in Mutex Lock",(uint32_t)status);
        return OSALSTATUS_FAILED;
    }
    phOsal_LogInfo((const uint8_t*)"Osal> Mutex Created");
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_MutexLock(void *hMutex)
{
    uint32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hMutex == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Mutex Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(pthread_mutex_lock(hMutex) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal>Error in Mutex Lock");
        return OSALSTATUS_INVALID_PARAMS;
    }

    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_MutexUnlock(void *hMutex)
{
    uint32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hMutex == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Mutex Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(pthread_mutex_unlock(hMutex) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal>Error in Mutex UnLock");
        return OSALSTATUS_INVALID_PARAMS;
    }

    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_MutexDelete(void *hMutex)
{
    uint32_t checkval;
    LOG_FUNCTION_ENTRY;
    if(hMutex == NULL)
    {
        phOsal_LogError((const uint8_t*)"Osal>Invalid Mutex Handle");
        return OSALSTATUS_INVALID_PARAMS;
    }

    if(pthread_mutex_destroy(hMutex) == -1)
    {
        phOsal_LogError((const uint8_t*)"Osal>Error in Mutex Destroy");
        return OSALSTATUS_INVALID_PARAMS;
    }

    free(hMutex);

    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

OSALSTATUS phOsal_Init(pphOsal_Config_t pOsalConfig)
{
    pphOsal_Config_t config = pOsalConfig;
    LOG_FUNCTION_ENTRY;
    LOG_FUNCTION_EXIT;
    return OSALSTATUS_SUCCESS;
}

void phOsal_Delay(uint32_t dwDelayInMs)
{
    usleep(dwDelayInMs*1000); /**< Converting milliseconds to Microseconds */
}

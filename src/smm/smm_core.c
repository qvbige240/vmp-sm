/**
 * History:
 * ================================================================
 * 2019-09-10 qing.zou created
 *
 */

#include <pthread.h>

#include "smm_core.h"

typedef struct _PrivInfo
{
    int                 cond;
    pthread_t           core_thread;
    pthread_mutex_t     core_mutex;
} PrivInfo;


void smm_core_done(void);

static void *smm_core_thread(void *arg)
{
    PrivInfo *thiz = (PrivInfo *)arg;

    VMP_LOGD("core test");

    while (thiz->cond)
    {
        sleep(5);
    }
    
    return NULL;
}

static int smm_core_start(PrivInfo *thiz)
{
    int ret = 0;
    pthread_mutexattr_t attr;

    if (!thiz)
        return -1;

    if (pthread_mutexattr_init(&attr) != 0)
        goto fail;
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
        goto fail;
    if (pthread_mutex_init(&thiz->core_mutex, &attr) != 0)
        goto fail;

    ret = pthread_create(&thiz->core_thread, NULL, smm_core_thread, (void *)thiz);
    if (ret != 0)
        VMP_LOGE("create thread 'smm_core_thread' failed");

fail:
    smm_core_done();
    return -1;
}

void smm_core_init(void)
{
    PrivInfo *thiz = calloc(1, sizeof(PrivInfo));
    thiz->cond = 1;
    smm_core_start(thiz);
}

void smm_core_done(void)
{

}

/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#include <pthread.h>

#include "smm_node_info.h"

#include "web_register.h"

typedef struct _PrivInfo
{
    SmmNodeInfoReq      req;
    SmmNodeInfoRsp      rsp;

    int                 cond;
} PrivInfo;

// extern int main_test(int argc, char **argv);

// void smm_core_done(void);

// static void *smm_core_thread(void *arg)
// {
//     PrivInfo *thiz = (PrivInfo *)arg;

//     VMP_LOGD("core test");

//     main_test(0, NULL);

//     while (thiz->cond)
//     {
//         sleep(5);
//     }

//     return NULL;
// }

// static int smm_core_start(PrivInfo *thiz)
// {
//     int ret = 0;
//     pthread_mutexattr_t attr;

//     if (!thiz)
//         return -1;

//     if (pthread_mutexattr_init(&attr) != 0)
//         goto fail;
//     if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
//         goto fail;
//     if (pthread_mutex_init(&thiz->core_mutex, &attr) != 0)
//         goto fail;

//     ret = pthread_create(&thiz->core_thread, NULL, smm_core_thread, (void *)thiz);
//     if (ret != 0)
//         VMP_LOGE("create thread 'smm_core_thread' failed");

// fail:
//     smm_core_done();
//     return -1;
// }

static int api_service_handle(void *p, void *request, void *response)
{
    //PrivInfo *thiz = p;
    ApiNodeInfoReq *req = request;
    ApiNodeInfoRsp *rsp = response;
    VMP_LOGD("req id = %d\n", req->id);

    //char *sname = "server node 01";
    rsp->id = req->id;
    rsp->count = 11;
    strncpy(rsp->name, "server node 01", sizeof(rsp->name));

    return 0;
}

static int node_info_register(PrivInfo *thiz)
{
    service_handler_t *service = calloc(1, sizeof(service_handler_t));
    service->ctx = thiz;
    service->pfn_callback = api_service_handle;

    web_node_info_register(thiz->req.web_server, service);

    return 0;
}

int smm_node_info_start(void *p)
{
    node_info_register(p);

    return 0;
}

void *smm_node_info_create(void *parent, SmmNodeInfoReq *req)
{
    PrivInfo *priv = calloc(1, sizeof(PrivInfo));
    if (priv)
    {
        priv->cond  = 1;
        priv->req   = *req;
    }
    return priv;
}

void smm_node_info_destroy(PrivInfo *thiz)
{
    if (thiz)
    {
        free(thiz);
    }
}

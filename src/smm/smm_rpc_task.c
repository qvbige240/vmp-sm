/**
 * History:
 * ================================================================
 * 2019-10-09 qing.zou created
 *
 */

#include <pthread.h>

#include "rpc_clnt.h"

#include "smm_rpc_task.h"

typedef struct _PrivInfo
{
    SmmRpcTaskReq           req;
    SmmRpcTaskRsp           rsp;

    vmp_rpclnt_t            *client;

    int                     cond;

    pthread_t               thread;
    pthread_mutex_t         mutex;
} PrivInfo;

static void rpc_client_start(PrivInfo *thiz)
{
    vmp_rpclnt_t *clnt = vmp_rpclnt_create();
    RpclntReq req = {0};
    req.port = 9876;
    strcpy(req.ip, "localhost");
    vmp_rpclnt_set(clnt, &req);
    vmp_rpclnt_start(clnt);
    thiz->client = clnt;
}

#include "clnt_demo.h"
static int client_demo_callback(void *p, int msg, void *arg)
{
    if (msg != 0) {
        VMP_LOGE("client_demo_callback failed");
        return -1;
    }
    ClntDemoRsp *rsp = arg;
    VMP_LOGD("demo call response:");
    VMP_LOGD(" id: %d", rsp->id);
    VMP_LOGD(" count: %d", rsp->count);
    VMP_LOGD(" memory: %ld", rsp->memory);
    return 0;
}
static int client_demo_call(vmp_rpclnt_t *thiz)
{
    if (thiz && thiz->clnt)
    {
        ClntDemoReq req = {0};
        req.id            = 1;

        req.ctx           = thiz;
        req.pfn_callback  = client_demo_callback;
        return rpc_clnt_demo(thiz, &req);
    }
    return 0;
}

#include "clnt_node_state.h"
static int client_node_state_callback(void *p, int msg, void *arg)
{
    if (msg != 0) {
        VMP_LOGE("client_node_state_callback failed");
        return -1;
    }
    ClntNodeStateRsp *rsp = arg;
    VMP_LOGD("node_state call response:");
    VMP_LOGD(" id: %d", rsp->id);
    VMP_LOGD(" index: %d", rsp->index);
    VMP_LOGD(" name: %s", rsp->name);
    VMP_LOGD(" count: %d", rsp->count);
    VMP_LOGD(" uplink: %ld", rsp->uplink);
    VMP_LOGD(" downlink: %ld", rsp->downlink);
    VMP_LOGD(" memory: %ld", rsp->memory);
    VMP_LOGD(" cpu: %lf", rsp->cpu);
    return 0;
}
static int client_node_state_call(vmp_rpclnt_t *thiz)
{
    if (thiz && thiz->clnt)
    {
        ClntNodeStateReq req = {0};
        req.id            = 1;

        req.ctx           = thiz;
        req.pfn_callback  = client_node_state_callback;
        return rpc_clnt_node_state(thiz, &req);
    }
    return 0;
}

static void *smm_rpc_task_thread(void *arg)
{
    PrivInfo *thiz = (PrivInfo *)arg;

    VMP_LOGD("rpc client start...");
    rpc_client_start(thiz);

    sleep(1);

    while (thiz->cond)
    {
        client_demo_call(thiz->client);

        client_node_state_call(thiz->client);

        sleep(5);
    }

    pthread_exit(0);

    return NULL;
}

int smm_rpc_task_start(void *p)
{
    int ret = -1;
    pthread_mutexattr_t attr;
    PrivInfo *thiz = p;

    if (!thiz)
        return -1;

    if (pthread_mutexattr_init(&attr) != 0)
        goto fail;
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
        goto fail;
    if (pthread_mutex_init(&thiz->mutex, &attr) != 0)
        goto fail;

    ret = pthread_create(&thiz->thread, NULL, smm_rpc_task_thread, (void *)thiz);
    if (ret != 0)
        VMP_LOGE("create thread 'smm_rpc_task_thread' failed");

    pthread_detach(thiz->thread);
fail:
    //smm_core_done();
    return ret;
}

// static int api_service_handle(void *p, void *request, void *response)
// {
//     //PrivInfo *thiz = p;
//     ApiDemoReq *req = request;
//     ApiDemoRsp *rsp = response;
//     VMP_LOGD("req id = %d\n", req->id);

//     //char *sname = "server node 01";
//     rsp->id     = req->id;
//     rsp->count  = 11;
//     strncpy(rsp->name, "server node 01", sizeof(rsp->name));

//     return 0;
// }

// static int rpc_task_register(PrivInfo *thiz)
// {
//     service_handler_t *service = calloc(1, sizeof(service_handler_t));
//     service->ctx            = thiz;
//     service->pfn_callback   = api_service_handle;

//     web_rpc_task_register(thiz->req.web, service);

//     return 0;
// }

// int smm_rpc_task_start(void *p)
// {
//     rpc_task_register(p);

//     return 0;
// }

void *smm_rpc_task_create(void *parent, SmmRpcTaskReq *req)
{
    PrivInfo *priv = calloc(1, sizeof(PrivInfo));
    if (priv)
    {
        priv->cond  = 1;
        priv->req   = *req;
    }
    return priv;
}

void smm_rpc_task_destroy(void *p)
{
    PrivInfo *thiz = p;
    if (thiz)
    {
        pthread_mutex_destroy(&thiz->mutex);

        free(thiz);
    }
}

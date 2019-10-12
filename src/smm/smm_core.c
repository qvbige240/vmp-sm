/**
 * History:
 * ================================================================
 * 2019-09-10 qing.zou created
 *
 */

#include <pthread.h>

#include "smm_core.h"

#include "web_server.h"

typedef struct _PrivInfo
{
    int                 cond;
    pthread_t           core_thread;
    pthread_mutex_t     core_mutex;

    void                *web_server;

    void                *web_demo;
    void                *node_info;
    void                *rpc_task;
} PrivInfo;

extern int main_test(int argc, char **argv);

void smm_core_done(void);

/** rpc task process **/
static int rpc_callback(void* p, int msg, void* arg)
{
    if (msg != 0) {
        VMP_LOGE("rpc_callback failed");
        return -1;
    }
    return 0;
}
static void task_rpc_start(PrivInfo* thiz)
{
    SmmRpcTaskReq req = {0};
    req.ctx     = thiz;
    req.pfncb   = rpc_callback;
    thiz->rpc_task = smm_rpc_task_create(thiz, &req);
    if (thiz->rpc_task) {
        smm_rpc_task_start(thiz->rpc_task);
    }
}

/** web demo **/
static void task_web_demo(PrivInfo *thiz)
{
    SmmDemoReq req = {0};
    req.web = thiz->web_server;
    thiz->web_demo = smm_demo_create(thiz, &req);
    if (thiz->web_demo)
    {
        smm_demo_start(thiz->web_demo);
    }
}

/** web node info **/
static void task_web_node_info(PrivInfo *thiz)
{
    SmmNodeInfoReq req = {0};
    req.web_server = thiz->web_server;
    thiz->node_info = smm_node_info_create(thiz, &req);
    if (thiz->node_info)
    {
        smm_node_info_start(thiz->node_info);
    }
}

/** web server **/
static int api_register_func(void *p, int msg, void *arg)
{
    task_web_demo(p);
    task_web_node_info(p);

    return 0;
}

static void task_web_server(PrivInfo *thiz)
{
    VMP_LOGD("web server start...");
    WebServerReq req = {0};
    req.port = 9090;
    req.func = api_register_func;
    req.ctx  = thiz;
    thiz->web_server = web_server_create(thiz, &req);
    if (thiz->web_server)
    {
        web_server_start(thiz->web_server);
    }
}

static void *smm_core_thread(void *arg)
{
    PrivInfo *thiz = (PrivInfo *)arg;

    VMP_LOGD("core test");

    //main_test(0, NULL);
    task_web_server(thiz);
    task_rpc_start(thiz);

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

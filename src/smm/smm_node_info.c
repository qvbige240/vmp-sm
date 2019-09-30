/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#include <pthread.h>

#include "smm_node_info.h"

#include "web_register.h"

#include "dao/dao_server_node.h"

typedef struct _PrivInfo
{
    SmmNodeInfoReq      req;
    SmmNodeInfoRsp      rsp;

    void                *node_table;

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

// typedef struct EntitySNodeInfo
// {
//     unsigned int        id;
//     char                name[64];
//     char                system[64];     /* system release type */
//     char                location[64];   /* place */
//     char                processor[64];  /* cpu model */
//     unsigned long       bandwidth;      /* network */
//     unsigned long       memory;         /* memory total KB */
// } EntitySNodeInfo;

static void test_server_node_add(void *p, int num)
{
    int i = 0, n = num;
    EntitySNodeInfo info = {0};
    PrivInfo *thiz = (PrivInfo *)p;

    for (i = 0; i < n; i++)
    {
        info.id = i;
        sprintf(info.name, "%s%d", "stream_server", i + 1);
        strcpy(info.system, "centos");
        strcpy(info.location, "chengdu");
        strcpy(info.processor, "i7 9900k");
        info.bandwidth = (1UL + i) << 20;
        info.memory = ((2UL + i) << 30);
        size_t slot = 0;
        dao_snode_add(thiz->node_table, &info, &slot);

        VMP_LOGD("add server id(%d) name(%s)", info.id, info.name);
        memset(&info, 0x00, sizeof(EntitySNodeInfo));
    }
}

/** node list interface **/
static int list_traverse_proc(void *p, visit_proc_cb proc, void *ctx)
{
    PrivInfo *thiz = (PrivInfo *)p;
    list_t *head = dao_snode_list_clone(thiz->node_table);

    da_object_t *obj;
    list_t *pos, *n;
    list_for_each_safe(pos, n, head)
    {
        obj = container_of(pos, da_object_t, node);
        if (obj)
        {
            list_del(pos);
            //print_object(obj);

            proc(ctx, obj->priv);

            free(obj);
        }
    }
    return 0;
}
static int api_handle_node_list(void *p, void *request, void *response)
{
    //PrivInfo *thiz = p;
    ApiNodeListReq *req = request;
    ApiNodeListRsp *rsp = response;
    //VMP_LOGD("req id = %d\n", req->id);

    //char *sname = "server node 01";
    //rsp->id = req->id;
    rsp->traverse = list_traverse_proc;

    return 0;
}

static int node_list_register(PrivInfo *thiz)
{
    service_handler_t *service = calloc(1, sizeof(service_handler_t));
    service->ctx = thiz;
    service->pfn_callback = api_handle_node_list;

    web_node_list_register(thiz->req.web_server, service);

    return 0;
}

/** node info interface **/
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
    test_server_node_add(p, 10); //...

    node_list_register(p);

    node_info_register(p);

    return 0;
}

void *smm_node_info_create(void *parent, SmmNodeInfoReq *req)
{
    PrivInfo *priv = calloc(1, sizeof(PrivInfo));
    if (priv)
    {
        priv->cond          = 1;
        priv->req           = *req;
        priv->node_table    = dao_snode_table_create();
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

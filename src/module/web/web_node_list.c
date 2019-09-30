/**
 * History:
 * ================================================================
 * 2019-09-12 qing.zou created
 *
 */

#include <onion/log.h>
#include <onion/onion.h>
#include <onion/dict.h>
#include <onion/handler.h>
#include <onion/request.h>
#include <onion/response.h>

#include "jansson.h"

#include "api/api_node_list.h"
#include "web_server.h"
#include "web_register.h"

typedef struct _PrivInfo
{
    ApiNodeListReq      req;
    ApiNodeListRsp      rsp;

    service_handler_t   *handler;

    char                *path;

    char                *data;
    json_t              *nodes_array;   /* server nodes*/

    int                 cond;
} PrivInfo;

// static void parameter_query(char *uri, const char *key, const char *value)
// {
//     printf("===== uri(%s) key, value ( %s %s)\n", uri, key, value);
// }

// static int query_param_parse(void *p, onion_request *req)
// {
//     const char *value;
//     PrivInfo *thiz = p;

//     //value = onion_request_get_session(req, "id");
//     value = onion_request_get_query(req, "id");
//     if (!value) goto param_parse_end;
//     thiz->req.id = atoi(value);
//     VMP_LOGD("id: %d", thiz->req.id);

//     return 0;

// param_parse_end:
//     VMP_LOGE("\n============ param parse error!!! ============\n");
//     return -1;
// }
    // unsigned int        index;
    // unsigned int        id;
    // char                name[64];
    // char                system[64];     /* system release type */
    // char                location[64];   /* place */
    // char                processor[64];  /* cpu model */
    // unsigned long       bandwidth;      /* network */
    // unsigned long       memory;         /* memory total KB */
static void* json_append_list(void *ctx, void *data)
{
    PrivInfo *thiz = ctx;
    json_t *snode = json_object();

    SvrNodeInfo *info = (SvrNodeInfo*)data;
    json_object_set_new(snode, "index", json_integer(info->index));
    json_object_set_new(snode, "id", json_integer(info->id));
    json_object_set_new(snode, "name", json_string(info->name));
    json_object_set_new(snode, "system", json_string(info->system));
    json_object_set_new(snode, "location", json_string(info->location));
    json_object_set_new(snode, "processor", json_string(info->processor));
    json_object_set_new(snode, "bandwidth", json_integer(info->bandwidth));
    json_object_set_new(snode, "memory", json_integer(info->memory));
    json_array_append(thiz->nodes_array, snode);
    
    json_decref(snode);
    return thiz->nodes_array;
}

static void response_json_create(PrivInfo *thiz)
{
    ApiNodeListRsp *rsp = &thiz->rsp;
    json_t *json_root = json_object();
    thiz->nodes_array = json_array();

    json_object_set_new(json_root, "type", json_string("stream_servers"));

    if (rsp->traverse)
        rsp->traverse(thiz->handler->ctx, json_append_list, thiz);

    json_object_set_new(json_root, "nodes", thiz->nodes_array);

    char *data_dump = json_dumps(json_root, 0);
    VMP_LOGD("response node list:\n%s", data_dump);

    thiz->data = calloc(1, strlen(data_dump) + 1);
    if (thiz->data)
        strcpy(thiz->data, data_dump);
    else
        VMP_LOGE("memory alloc failed: response_json_create");

    free(data_dump);
    json_decref(json_root);
}

static int handle_node_list(void *p, onion_request *req, onion_response *res)
{
    int ret = 0;
    // carnet/sm/lbs/node_list
    VMP_LOGD("\n========== process uri %s ==========", onion_request_get_fullpath(req));
    printf("query 1: %s\n", onion_request_get_query(req, "1"));
    printf("query 100: %s\n", onion_request_get_query(req, "100"));
    printf("query id: %s\n", onion_request_get_query(req, "id"));
    //onion_response_set_length(res, 11);

    PrivInfo *thiz = calloc(1, sizeof(PrivInfo));
    thiz->path = (char *)onion_request_get_fullpath(req);

    // int ret = query_param_parse(thiz, req);
    // if (ret == 0) {
    //     // callback
    // } else {
    //     VMP_LOGE("handle_node_list failed!");
    // }

    thiz->handler = (service_handler_t *)p;
    service_handler_t *service = thiz->handler;
    ret = service->pfn_callback(service->ctx, &thiz->req, &thiz->rsp);
    if (ret != 0) {
        VMP_LOGE("get response failed");
    }

    response_json_create(thiz);
    onion_response_write0(res, thiz->data);

    return OCS_PROCESSED;
}

int web_node_list_register(void *server, void *args)
{
    return web_handler_register(server, WEB_URL_NODE_LIST, handle_node_list, args);
}

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

#include "api/api_node_info.h"
#include "web_server.h"
#include "web_register.h"

typedef struct _PrivInfo
{
    ApiNodeInfoReq      req;
    ApiNodeInfoRsp      rsp;

    char                *path;

    char                *data;

    int                 cond;
} PrivInfo;

static void parameter_query(char *uri, const char *key, const char *value)
{
    printf("===== uri(%s) key, value ( %s %s)\n", uri, key, value);
}

static int query_param_parse(void *p, onion_request *req, char **err_msg)
{
    const char *value;
    PrivInfo *thiz = p;

    //value = onion_request_get_session(req, "id");
    value = onion_request_get_query(req, "index");
    if (!value) goto param_parse_end;
    thiz->req.index = atoi(value);
    VMP_LOGD("index: %d", thiz->req.index);

    return 0;

param_parse_end:
    VMP_LOGE("\n============ param parse error!!! ============\n");
    if (err_msg)
    {
        *err_msg = "param index valid";
    }
    return -1;
}

#if 0
static void response_json_create(PrivInfo *thiz)
{
    const char *data = "{\"id\":\"1\",\"count\":10}";
    thiz->data = calloc(1, strlen(data) + 1);
    if (thiz->data)
        strcpy(thiz->data, data);
    else
        VMP_LOGE("memory alloc failed: response_json_create");
}
#else
static void response_json_create_failed(PrivInfo *thiz, int code, char *msg)
{
    json_t *json_root = json_object();

    json_object_set_new(json_root, "error_code", json_integer(code));
    //if (msg)
    json_object_set_new(json_root, "error_msg", json_string(msg));

    char *data_dump = json_dumps(json_root, 0);
    VMP_LOGD("response node info:\n%s", data_dump);

    thiz->data = calloc(1, strlen(data_dump) + 1);
    if (thiz->data)
        strcpy(thiz->data, data_dump);
    else
        VMP_LOGE("memory alloc failed: response_json_create");

    free(data_dump);
    json_decref(json_root);
}
static void response_json_create(PrivInfo *thiz)
{
    ApiNodeInfoRsp *rsp = &thiz->rsp;
    json_t *json_root = json_object();

    json_object_set_new(json_root, "index", json_integer(rsp->index));
    json_object_set_new(json_root, "name", json_string(rsp->name));
    json_object_set_new(json_root, "system", json_string(rsp->system));
    json_object_set_new(json_root, "location", json_string(rsp->location));
    json_object_set_new(json_root, "processor", json_string(rsp->processor));
    json_object_set_new(json_root, "bandwidth", json_integer(rsp->bandwidth));
    json_object_set_new(json_root, "memory", json_integer(rsp->memory));

    char *data_dump = json_dumps(json_root, 0);
    VMP_LOGD("response node info:\n%s", data_dump);

    thiz->data = calloc(1, strlen(data_dump) + 1);
    if (thiz->data)
        strcpy(thiz->data, data_dump);
    else
        VMP_LOGE("memory alloc failed: response_json_create");

    free(data_dump);
    json_decref(json_root);
}
#endif

static int handle_node_info(void *p, onion_request *req, onion_response *res)
{
    // carnet/sm/lbs/node_info
    VMP_LOGD("\n========== process uri %s ==========", onion_request_get_fullpath(req));
    printf("query 1: %s\n", onion_request_get_query(req, "1"));
    printf("query 100: %s\n", onion_request_get_query(req, "100"));
    printf("query id: %s\n", onion_request_get_query(req, "id"));
    //onion_response_set_length(res, 11);

    PrivInfo *thiz = calloc(1, sizeof(PrivInfo));
    thiz->path = (char *)onion_request_get_fullpath(req);

    char *err_msg = NULL;
    int ret = query_param_parse(thiz, req, &err_msg);
    if (ret == 0) {
        // callback
    } else {
        VMP_LOGE("handle_node_info failed!");
        response_json_create_failed(thiz, ret, err_msg);
        goto end;
    }

    service_handler_t *service = (service_handler_t *)p;
    ret = service->pfn_callback(service->ctx, &thiz->req, &thiz->rsp);
    if (ret != 0) {
        VMP_LOGE("get response failed");
        response_json_create_failed(thiz, ret, "index query failed");
        goto end;
    }

    response_json_create(thiz);
end:
    onion_response_write0(res, thiz->data);

    if (0)
    {
        const char *path = onion_request_get_path(req);
        onion_dict_preorder(onion_request_get_query_dict(req), parameter_query, (void *)path);
        // onion_response_write0(res, "Connection");
        // onion_response_write(res, ": ", 2);
        // onion_response_write0(res, "keep-alive");
        // onion_response_write(res, "\r\n", 2);
        // onion_response_write(res, "\r\n", 2);

        onion_response_write0(res, "Hello world");
        if (onion_request_get_query(req, "1"))
        {
            onion_response_printf(res, "<p>Path: %s", onion_request_get_query(req, "1"));
        }
        onion_response_printf(res, "<p>Client addr: %s", onion_request_get_client_description(req));
    }
    return OCS_PROCESSED;
}

int web_node_info_register(void *server, void *args)
{
    return web_handler_register(server, WEB_URL_NODE_INFO, handle_node_info, args);
}

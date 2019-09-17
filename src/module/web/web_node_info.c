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

static int query_param_parse(void *p, onion_request *req)
{
    const char *value;
    PrivInfo *thiz = p;

    //value = onion_request_get_session(req, "id");
    value = onion_request_get_query(req, "id");
    if (!value) goto param_parse_end;
    thiz->req.id = atoi(value);
    VMP_LOGD("id: %d", thiz->req.id);

    return 0;

param_parse_end:
    VMP_LOGE("\n============ param parse error!!! ============\n");
    return -1;
}

static void response_json_create(PrivInfo *thiz)
{
    const char *data = "{\"id\":\"1\",\"count\":10}";
    thiz->data = calloc(1, strlen(data) + 1);
    if (thiz->data)
        strcpy(thiz->data, data);
    else
        VMP_LOGE("memory alloc failed: response_json_create");
}

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

    int ret = query_param_parse(thiz, req);
    if (ret == 0) {
        // callback
    } else {
        VMP_LOGE("handle_node_info failed!");
    }

    service_handler_t *service = (service_handler_t *)p;
    ret = service->pfn_callback(service->ctx, &thiz->req, &thiz->rsp);
    if (ret != 0) {
        VMP_LOGE("get response failed");
    }

    response_json_create(thiz);
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
    //return web_handler_register(server, WEB_URL_NODE_INFO, handle_node_info, args);
    return web_handler_register(server, WEB_URL_HELLO, handle_node_info, args);
}

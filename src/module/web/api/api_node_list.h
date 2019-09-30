/**
 * History:
 * ================================================================
 * 2019-09-12 qing.zou created
 *
 */

#ifndef API_NODE_LIST_H
#define API_NODE_LIST_H

#include "api_typedef.h"
#include "model/model_server.h"

VMP_BEGIN_DELS

typedef void* (*visit_proc_cb)(void *ctx, void *data);
typedef int (*list_traverse_cb)(void *p, visit_proc_cb proc, void *ctx);

typedef struct SvrNodeInfo
{
    svr_node_object();

    // unsigned int        index;
    // unsigned int        id;
    // char                name[64];
    // char                system[64];     /* system release type */
    // char                location[64];   /* place */
    // char                processor[64];  /* cpu model */
    // unsigned long       bandwidth;      /* network */
    // unsigned long       memory;         /* memory total KB */
} SvrNodeInfo;

typedef struct _ApiNodeListReq
{
} ApiNodeListReq;

typedef struct _ApiNodeListRsp
{
    char                type[32];

    void                *ctx;
    list_traverse_cb    traverse;
} ApiNodeListRsp;


VMP_END_DELS

#endif // API_NODE_LIST_H

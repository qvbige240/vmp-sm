/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#ifndef SMM_NODEINFO_H
#define SMM_NODEINFO_H

#include "smm_typedef.h"

VMP_BEGIN_DELS

typedef struct _SmmNodeInfoReq
{
    unsigned int            id;

    void                    *web_server;

    void                    *ctx;
    vmp_callback_func       pfncb;
} SmmNodeInfoReq;

typedef struct _SmmNodeInfoRsp
{
    unsigned int            num;
} SmmNodeInfoRsp;

void *smm_node_info_create(void *parent, SmmNodeInfoReq *req);
int smm_node_info_start(void *p);

VMP_END_DELS

#endif // SMM_NODEINFO_H

/**
 * History:
 * ================================================================
 * 2019-10-09 qing.zou created
 *
 */

#ifndef SMM_RPC_TASK_H
#define SMM_RPC_TASK_H

#include "smm_typedef.h"

VMP_BEGIN_DELS

typedef struct _SmmRpcTaskReq
{
    void                    *core;

    void                    *ctx;
    vmp_callback_func       pfncb;
} SmmRpcTaskReq;

typedef struct _SmmRpcTaskRsp
{
} SmmRpcTaskRsp;

void *smm_rpc_task_create(void *parent, SmmRpcTaskReq *req);
int smm_rpc_task_start(void *p);
void smm_rpc_task_destroy(void *p);

VMP_END_DELS

#endif // SMM_RPC_TASK_H

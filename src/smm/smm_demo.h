/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#ifndef SMM_DEMO_H
#define SMM_DEMO_H

#include "smm_typedef.h"

VMP_BEGIN_DELS

typedef struct _SmmDemoReq
{
    unsigned int            id;

    void                    *web;

    void                    *ctx;
    vmp_callback_func       pfncb;
} SmmDemoReq;

typedef struct _SmmDemoRsp
{
    unsigned int            num;
} SmmDemoRsp;

void *smm_demo_create(void *parent, SmmDemoReq *req);
int smm_demo_start(void *p);

VMP_END_DELS

#endif // SMM_DEMO_H

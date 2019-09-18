/**
 * History:
 * ================================================================
 * 2019-09-12 qing.zou created
 *
 */

#ifndef API_NODE_INFO_H
#define API_NODE_INFO_H

#include "api_typedef.h"

VMP_BEGIN_DELS


typedef struct _ApiNodeInfoReq
{
    unsigned int        id;
} ApiNodeInfoReq;

typedef struct _ApiNodeInfoRsp
{
    unsigned int        id;

    unsigned int        count;

    char                name[64];
} ApiNodeInfoRsp;


VMP_END_DELS

#endif // API_NODE_INFO_H

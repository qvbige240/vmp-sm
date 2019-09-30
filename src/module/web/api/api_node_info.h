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
    char                system[64];     /* system release type */
    char                location[64];   /* place */
    char                processor[64];  /* cpu model */
    unsigned long       bandwidth;      /* network */
    unsigned long       memory;         /* memory total KB */
} ApiNodeInfoRsp;


VMP_END_DELS

#endif // API_NODE_INFO_H

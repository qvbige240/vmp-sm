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
    unsigned int        index;
} ApiNodeInfoReq;

typedef struct _ApiNodeInfoRsp
{
    unsigned int        index;
    unsigned int        id;
    char                name[64];
    char                system[64];     /* system release type */
    char                location[64];   /* place */
    char                processor[64];  /* cpu model */
    unsigned long       bandwidth;      /* network */
    unsigned long       memory;         /* memory total KB */

    unsigned int        count;
    unsigned long       uplink;
    unsigned long       downlink;
    unsigned long       memused;
    double              cpu;
} ApiNodeInfoRsp;


VMP_END_DELS

#endif // API_NODE_INFO_H

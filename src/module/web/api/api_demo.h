/**
 * History:
 * ================================================================
 * 2019-09-30 qing.zou created
 *
 */

#ifndef API_DEMO_H
#define API_DEMO_H

#include "api_typedef.h"

VMP_BEGIN_DELS


typedef struct _ApiDemoReq
{
    unsigned int        id;
} ApiDemoReq;

typedef struct _ApiDemoRsp
{
    unsigned int        id;

    unsigned int        count;

    char                name[64];
    char                system[64];     /* system release type */
    char                location[64];   /* place */
    char                processor[64];  /* cpu model */
    unsigned long       bandwidth;      /* network */
    unsigned long       memory;         /* memory total KB */
} ApiDemoRsp;


VMP_END_DELS

#endif // API_DEMO_H

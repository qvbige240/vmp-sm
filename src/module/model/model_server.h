/**
 * History:
 * ================================================================
 * 2019-09-29 qing.zou created
 *
 */

#ifndef MODEL_SERVER_H
#define MODEL_SERVER_H

#include "vmp.h"

VMP_BEGIN_DELS

#define svr_node_object()       \
    unsigned int        index;                                          \
    unsigned int        id;                                             \
    char                name[64];                                       \
    char                system[64];     /* system release type */       \
    char                location[64];   /* place */                     \
    char                processor[64];  /* cpu model */                 \
    unsigned long       bandwidth;      /* network */                   \
    unsigned long       memory;         /* memory total KB */



VMP_END_DELS

#endif // MODEL_SERVER_H

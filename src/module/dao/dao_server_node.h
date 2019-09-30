/**
 * History:
 * ================================================================
 * 2019-09-27 qing.zou created
 *
 */

#ifndef DAO_SERVER_NODE_H
#define DAO_SERVER_NODE_H

#include "vmp.h"
#include "model/model_server.h"

VMP_BEGIN_DELS

typedef struct EntitySNodeInfo
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
} EntitySNodeInfo;

typedef struct EntitySNodeState
{
    unsigned int        id;
    char                name[64];
    unsigned int        count;          /* stream connection count */
    unsigned long       uplink;         /* network uplink (KB) */
    unsigned long       downlink;       /* network downlink (KB) */
    unsigned long       memory;         /* memory used (KB) */
    double              cpu;            /* cpu utilization */
} EntitySNodeState;

void *dao_snode_table_create(void);
int dao_snode_add(vpk_dalist_t *list, EntitySNodeInfo *info, size_t *out_slot);
int dao_snode_delete(vpk_dalist_t *list, size_t slot);
int dao_snode_modify_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeInfo *info);
int dao_snode_get_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeInfo *info);
void dao_snode_table_destroy(vpk_dalist_t *list);
list_t *dao_snode_list_clone(vpk_dalist_t *list);

VMP_END_DELS

#endif // DAO_SERVER_NODE_H

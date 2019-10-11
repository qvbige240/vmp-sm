/**
 * History:
 * ================================================================
 * 2019-09-27 qing.zou created
 *
 */

#include "dao_server_node.h"

typedef struct _PrivInfo
{
    EntitySNodeInfo     info;
    EntitySNodeState    state;
} PrivInfo;

static int object_destroy(void *ctx, void *data)
{
    da_object_t *object = data;
    if (object)
    {
        VMP_LOGD("free object %p slot[%d]", object, object->slot);
        free(object);
    }
    return 0;
}
void *dao_snode_table_create(void)
{
    vpk_dalist_t *list = vpk_dalist_create(16, object_destroy, NULL);
    if (!list)
        VMP_LOGE("table create failed");
    return list;
}

int dao_snode_add(vpk_dalist_t *list, EntitySNodeInfo *info, size_t *out_slot)
{
    int ret = -1;
    da_object_t *object = calloc(1, sizeof(da_object_t) + sizeof(PrivInfo));
    if (object)
    {
        DECL_PRIV(object, priv);
        memcpy(&priv->info, info, sizeof(EntitySNodeInfo));

        size_t slot = 0;
        ret = vpk_dalist_add(list, &slot, object);
        if (ret != 0)
        {
            VMP_LOGE("add server node object %p failed, ret = %d", object, ret);
            free(object);
            return -1;
        }
        else
        {
            priv->info.index = slot;
            object->slot = slot;
            if (out_slot)
                *out_slot = slot;
            VMP_LOGD("add object %p to slot[%d]", object, slot);
        }
    }

    return ret;
}

int dao_snode_delete(vpk_dalist_t *list, size_t slot)
{
    int ret = vpk_dalist_delete(list, slot);
    if (ret != 0)
        VMP_LOGE("server node delete failed, slot %d", slot);
    return ret;
}

static int info_modify_cb(void *ctx, void *data)
{
    da_object_t *object = data;
    if (object)
    {
        DECL_PRIV(object, priv);
        memcpy(&priv->info, ctx, sizeof(EntitySNodeInfo));
        // strcat(priv->info.name, "modified first");
        // VMP_LOGD("object  id name memory  slot[%d]( %d %s %lu )",
        //       object->slot, priv->info.id, priv->info.name, priv->info.memory);
    }
    return 0;
}
int dao_snode_modify_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeInfo *info)
{
    int ret = vpk_dalist_modify_by_index(list, slot, info_modify_cb, info);
    if (ret != 0)
        VMP_LOGE("vpk_dalist_modify_by_index failed, slot %d", slot);

    return ret;
}

int dao_snode_get_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeInfo *info)
{
    da_object_t *object = NULL;
    int ret = vpk_dalist_get_by_index(list, slot, &object);
    if (ret != 0)
        VMP_LOGE("vpk_dalist_get_by_index failed, slot %d", slot);

    if (object)
    {
        DECL_PRIV(object, priv);
        memcpy(info, &priv->info, sizeof(EntitySNodeInfo));
    }

    return ret;
}

static int server_node_id_compare(void *ctx, void *data)
{
    EntitySNodeInfo *info = ctx;
    da_object_t *object = data;
    if (object && info)
    {
        DECL_PRIV(object, priv);
        if (priv->info.id == info->id)
        {
            memcpy(info, &priv->info, sizeof(EntitySNodeInfo));
            return 0;
        }
    }

    return -1;
}
int dao_snode_find_by_id(vpk_dalist_t *list, EntitySNodeInfo *info)
{
    int index = 0;
    index = vpk_dalist_find(list, server_node_id_compare, info);
    if (index < 0)
        VMP_LOGE("find server by id failed, ret = %d", index);
    return index;
}

void dao_snode_table_destroy(vpk_dalist_t *list)
{
    vpk_dalist_destroy(list);
}

list_t *dao_snode_list_clone(vpk_dalist_t *list)
{
    return vpk_dalist_clone_list(list, sizeof(da_object_t) + sizeof(PrivInfo));
}

static int state_modify_cb(void *ctx, void *data)
{
    da_object_t *object = data;
    if (object)
    {
        DECL_PRIV(object, priv);
        memcpy(&priv->state, ctx, sizeof(EntitySNodeState));
        // strcat(priv->state.name, "modified first");
        // VMP_LOGD("object  id name memory  slot[%d]( %d %s %lu )",
        //       object->slot, priv->state.id, priv->state.name, priv->state.memused);
    }
    return 0;
}
int dao_snode_status_modify_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeState *state)
{
    int ret = vpk_dalist_modify_by_index(list, slot, state_modify_cb, state);
    if (ret != 0)
        VMP_LOGE("dao_snode_status_modify_by_index failed, slot %d", slot);

    return ret;
}

int dao_snode_status_get_by_index(vpk_dalist_t *list, size_t slot, EntitySNodeState *state)
{
    da_object_t *object = NULL;
    int ret = vpk_dalist_get_by_index(list, slot, &object);
    if (ret != 0)
        VMP_LOGE("vpk_dalist_get_by_index failed, slot %d", slot);

    if (object)
    {
        DECL_PRIV(object, priv);
        memcpy(state, &priv->state, sizeof(EntitySNodeState));
    }

    return ret;
}

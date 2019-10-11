/**
 * History:
 * ================================================================
 * 2019-09-23 qing.zou created
 *
 */

#include "cache_table.h"

typedef struct _PrivInfo
{
    void        *node_table;
} PrivInfo;

static int cache_table_callback(void *p, int msg, void *arg);
static int cache_table_get(void *p, int id, void *data, int size)
{
    vmp_object_t *cache = p;
    PrivInfo *thiz = cache->private;
    CacheDataObject *object = data;

    switch (id)
    {
    case CACHE_TABLE_NODE:
        object->data = thiz->node_table;
        break;
    default:
        VMP_LOGW("get target id(%d) not exist!", id);
        break;
    }
    return 0;
}
static int cache_table_set(void *p, int id, const void *data, int size)
{
    vmp_object_t *cache = p;
    PrivInfo *thiz = cache->private;

    switch (id)
    {
    case CACHE_TABLE_NODE:
        thiz->node_table = data;
        break;
    default:
        VMP_LOGW("set target id(%d) not exist!", id);
        break;
    }
    return 0;
}
static int cache_table_start(void *p)
{
    PrivInfo *thiz = ((vmp_object_t *)p)->private;
    return 0;
}
static int cache_table_stop(void *p)
{
    return 0;
}

vmp_object_t *cache_table_create(void *parent, void *req)
{
    vmp_object_t *p = calloc(1, sizeof(vmp_object_t) + sizeof(PrivInfo));
    if (p)
    {
        p->private      = (void *)p + sizeof(vmp_object_t);
        p->pfn_get      = cache_table_get;
        p->pfn_set      = cache_table_set;
        p->pfn_start    = cache_table_start;
        p->pfn_stop     = cache_table_stop;

        //PrivInfo *priv  = p->private;
        //priv->req       = *req;
    }

    return p;
}

void cache_table_destroy(vmp_object_t *p)
{
    if (p)
    {
        free(p);
    }
}

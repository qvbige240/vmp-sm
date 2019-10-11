/**
 * History:
 * ================================================================
 * 2019-09-23 qing.zou created
 *
 */

#include "cache.h"
#include "cache_table.h"

typedef struct _PrivInfo
{
    void            *table;
} PrivInfo;

static int cache_callback(void *p, int msg, void *arg);
static int cache_get(void *p, int id, void *data, int size)
{
    vmp_object_t *cache = p;
    PrivInfo *thiz = cache->private;

    if (id > CACHE_TABLE_BEGIN && id < CACHE_TABLE_END)
    {
        vmp_object_t *table = thiz->table;
        table->pfn_get(table, id, data, size);
    }
    return 0;
}
static int cache_set(void *p, int id, const void *data, int size)
{
    vmp_object_t *cache = p;
    PrivInfo *thiz = cache->private;

    if (id > CACHE_TABLE_BEGIN && id < CACHE_TABLE_END)
    {
        vmp_object_t *table = thiz->table;
        table->pfn_set(table, id, data, size);
    }
    return 0;
}

int cache_start(void *p)
{
    PrivInfo *thiz = ((vmp_object_t *)p)->private;

    if (thiz->table == NULL)
    {
        thiz->table = cache_table_create(NULL, NULL);
    }

    return 0;
}
static int cache_stop(void *p)
{
    return 0;
}

vmp_object_t *cache_create(void *parent, CacheReq *req)
{
    vmp_object_t *p = calloc(1, sizeof(vmp_object_t) + sizeof(PrivInfo));
    if (p)
    {
        p->private      = (void *)p + sizeof(vmp_object_t);
        p->pfn_get      = cache_get;
        p->pfn_set      = cache_set;
        p->pfn_start    = cache_start;
        p->pfn_stop     = cache_stop;

        //PrivInfo *priv  = p->private;
        //priv->req       = *req;
    }

    cache_start(p);

    return p;
}

void cache_destroy(vmp_object_t *p)
{
    if (p)
    {
        PrivInfo *thiz = p->private;
        if (thiz->table == NULL)
            cache_table_destroy(thiz->table);

        free(p);
    }
}

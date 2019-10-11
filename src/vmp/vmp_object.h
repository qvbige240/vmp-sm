/**
 * History:
 * ================================================================
 * 2019-09-23 qing.zou created
 *
 */

#ifndef VMP_OBJECT_H
#define VMP_OBJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*object_get)(void *p, int id, void *data, int size);
typedef int (*object_set)(void *p, int id, const void *data, int size);
typedef int (*object_start)(void *p);
typedef int (*object_stop)(void *p);
typedef int (*object_callback)(void *p, int msg, void *arg);

typedef struct vmp_object_s
{
    object_get          pfn_get;
    object_set          pfn_set;
    object_start        pfn_start;
    object_stop         pfn_stop;
    object_callback     pfn_cb;

    void                *private;
    void                *parent;
} vmp_object_t;

#ifdef __cplusplus
}
#endif

#endif // VMP_OBJECT_H

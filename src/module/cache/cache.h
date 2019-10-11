/**
 * History:
 * ================================================================
 * 2019-09-23 qing.zou created
 *
 */

#ifndef CACHE_H
#define CACHE_H

#include "vmp.h"
#include "cache_typedef.h"

VMP_BEGIN_DELS

typedef struct _CacheReq
{

} CacheReq;

typedef struct _CacheRsp
{
    
} CacheRsp;

vmp_object_t *cache_create(void *parent, CacheReq *req);
int cache_start(void *p);
void cache_destroy(vmp_object_t *p);

VMP_END_DELS

#endif // CACHE_H

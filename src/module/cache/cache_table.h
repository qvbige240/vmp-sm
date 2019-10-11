/**
 * History:
 * ================================================================
 * 2019-09-23 qing.zou created
 *
 */

#ifndef CACHE_TABLE_H
#define CACHE_TABLE_H

#include "vmp.h"
#include "cache_typedef.h"

VMP_BEGIN_DELS

vmp_object_t *cache_table_create(void *parent, void *req);
void cache_table_destroy(vmp_object_t *p);

VMP_END_DELS

#endif // CACHE_TABLE_H

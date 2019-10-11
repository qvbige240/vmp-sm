/**
 * History:
 * ================================================================
 * 2019-10-09 qing.zou created
 *
 */

#ifndef CACHE_TYPEDEF_H
#define CACHE_TYPEDEF_H

#include "vmp.h"

VMP_BEGIN_DELS

typedef enum
{
    CACHE_TABLE_BEGIN = 100,

    CACHE_TABLE_NODE,

    CACHE_TABLE_END,
} CacheTableType;

typedef struct _CacheDataObject
{
    void        *data;
} CacheDataObject;


VMP_END_DELS

#endif // CACHE_TYPEDEF_H

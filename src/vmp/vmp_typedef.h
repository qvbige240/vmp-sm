/**
 * History:
 * ================================================================
 * 2019-08-15 qing.zou created
 *
 */
#ifndef VMP_TYPEDEF_H
#define VMP_TYPEDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vpk_darray_list.h"

#ifdef HAVE_VPK

  #include "vpk/vpk.h"

  #define VMP_BEGIN_DELS  VPK_BEGIN_DELS
  #define VMP_END_DELS    VPK_END_DELS

#else

  #ifdef __cplusplus
    #define VMP_BEGIN_DELS extern "C" {
    #define VMP_END_DELS }
  #else
    #define VMP_BEGIN_DELS
    #define VMP_END_DELS
  #endif

#endif  //HAVE_VPK

typedef int (*vmp_callback_func)(void *p, int msg, void *arg);

#endif // VMP_TYPEDEF_H

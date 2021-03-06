/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#ifndef WEB_HANDLER_H
#define WEB_HANDLER_H

#include "vmp.h"

VMP_BEGIN_DELS

#define WEB_URL_HELLO           "carnet/sm/lbs/hello"
#define WEB_URL_DEMO            "carnet/sm/lbs/demo"    // http://127.0.0.1:9090/carnet/sm/lbs/demo?id=3
#define WEB_URL_NODE_INFO       "carnet/sm/lbs/node_info" // http://127.0.0.1:9090/carnet/sm/lbs/node_info?index=1
#define WEB_URL_NODE_LIST       "carnet/sm/lbs/node_list" // http://127.0.0.1:9090/carnet/sm/lbs/node_list


typedef struct web_handler_s
{
    struct web_handler_s    *sc_next;
    char                    *sc_urls;
    void                    *sc_args;
    void                    *handler;
} web_handler_t;


VMP_END_DELS

#endif // WEB_HANDLER_H

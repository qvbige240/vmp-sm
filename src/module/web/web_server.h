/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "vmp.h"
#include "web_handler.h"

VMP_BEGIN_DELS

typedef int (*web_register_func)(void *p, int msg, void *arg);

typedef struct _WebServerReq
{
    unsigned short          port;

    web_register_func       func;

    void                    *ctx;
    vmp_callback_func       pfncb;
} WebServerReq;

typedef struct _WebServerRsp
{
    void                    *web;
} WebServerRsp;

void *web_server_create(void *parent, WebServerReq *req);
int web_server_start(void *web);

int web_handler_register(void *web, char *url, void *handler, void *args);

VMP_END_DELS

#endif // WEB_SERVER_H

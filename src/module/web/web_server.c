/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#include <pthread.h>

#include <onion/log.h>
#include <onion/onion.h>
#include <onion/dict.h>
#include <onion/handler.h>
#include <onion/request.h>
#include <onion/response.h>

#include "web_server.h"

typedef struct _PrivInfo
{
    WebServerReq        req;
    WebServerRsp        rsp;

    onion               *server;
    onion_url           *urls;

    web_handler_t       *sc_head;

    int                 cond;
    
    pthread_t           thread;
} PrivInfo;

static int url_add(PrivInfo *thiz)
{
    web_handler_t *s;
    for (s = thiz->sc_head; s != NULL; s = s->sc_next)
    {
        if (s->sc_urls && s->handler)
            onion_url_add_with_data(thiz->urls, s->sc_urls, s->handler, s->sc_args, NULL);
        //onion_url_add(thiz->urls, s->sc_urls, s->handler);
        //onion_url_add(thiz->urls, "carnet/sm/lbs/hello", hello);
    }

    return 0;
}

static void *web_server_thread(void *arg)
{
    PrivInfo *thiz = (PrivInfo *)arg;

    VMP_LOGD("web server test");

    thiz->server = onion_new(O_POOL);
    if (thiz->server)
    {
        char port[8] = {0};
        sprintf(port, "%d", thiz->req.port);
        //onion_set_timeout(thiz->server, 5000);
        onion_set_hostname(thiz->server, "0.0.0.0");
        onion_set_port(thiz->server, port);

        thiz->urls = onion_root_url(thiz->server);
        //onion_url_add(thiz->urls, "carnet/sm/lbs/hello", hello);
        if (thiz->req.func)
            thiz->req.func(thiz->req.ctx, 0, NULL);

        url_add(thiz);

        VMP_LOGI("Listening at http://0.0.0.0:%s", port);
        onion_listen(thiz->server);

        onion_free(thiz->server);
    }

    pthread_exit(0);

    return NULL;
}

int web_server_start(void *p)
{
    int ret = 0;
    PrivInfo *thiz = p;

    if (!thiz) return -1;

    // pthread_mutexattr_t attr;
    // if (pthread_mutexattr_init(&attr) != 0)
    //     goto fail;
    // if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0)
    //     goto fail;
    // if (pthread_mutex_init(&thiz->core_mutex, &attr) != 0)
    //     goto fail;

    ret = pthread_create(&thiz->thread, NULL, web_server_thread, (void *)thiz);
    if (ret != 0)
        VMP_LOGE("create thread 'web_server_thread' failed");

    pthread_detach(thiz->thread);
    
    //web_server_done();
    return 0;
}

void *web_server_create(void *parent, WebServerReq *req)
{
    PrivInfo *priv = calloc(1, sizeof(PrivInfo));
    if (priv)
    {
        priv->cond  = 1;
        priv->req   = *req;
    }
    return priv;
}

void web_server_destroy(PrivInfo *thiz)
{
    if (thiz)
    {
        free(thiz);
    }
}

/** url handler regisger **/
static web_handler_t *web_handler_find(void *p, char *url)
{
    PrivInfo *thiz = p;
    web_handler_t *s;

    if (!url)
    {
        VMP_LOGE("null pointer");
        return -1;
    }

    for (s = thiz->sc_head; s != NULL; s->sc_next)
    {
        if (strcmp(s->sc_urls, url) == 0)
            break;
    }

    return s;
}

int web_handler_register(void *web, char *url, void *handler, void *args)
{
    PrivInfo *thiz = web;
    web_handler_t *s;

    if (!web || !url || !handler)
    {
        VMP_LOGE("null pointer");
        return -1;
    }

    if ((s = web_handler_find(web, url)) != NULL)
    {
        if (s->handler == handler)
            return 0;
        VMP_LOGW("path: %s have been registered!");
        return 1;
    }
    s = calloc(1, sizeof(web_handler_t));
    if (s == NULL)
    {
        VMP_LOGE("memory alloc failed");
        return -1;
    }
    s->sc_urls      = url;
    s->sc_args      = args;
    s->handler      = handler;
    s->sc_next      = thiz->sc_head;
    thiz->sc_head   = s;

    return 0;
}

/**
  Onion HTTP server library
  Copyright (C) 2010-2018 David Moreno Montero and others

  This library is free software; you can redistribute it and/or
  modify it under the terms of, at your choice:

  a. the Apache License Version 2.0.

  b. the GNU General Public License as published by the
  Free Software Foundation; either version 2.0 of the License,
  or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of both licenses, if not see
  <http://www.gnu.org/licenses/> and
  <http://www.apache.org/licenses/LICENSE-2.0>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <onion/log.h>
#include <onion/onion.h>
#include <onion/dict.h>
#include <onion/handler.h>
#include <onion/request.h>
#include <onion/response.h>

#include <pthread.h>
static unsigned long long get_thread_id(void)
{
	union {
		pthread_t thr;
		unsigned long long id;
	} r;
	memset(&r, 0, sizeof(r));
	r.thr = pthread_self();
	return r.id;
}

/**
 * @short Asks a question and returns it.
 * 
 * The returned pointer must be freed by user.
 */
char *ask_question(const char *question) {
  printf("(%p)Answer for path '%s': ", get_thread_id(), question);
  char *ret = calloc(1, 1024);
  if (!fgets(ret, 1024, stdin)) {
    free(ret);
    return NULL;
  }
  return ret;
}

/// Format query string a little bit to understand the query itself
//void format_query(const char *key, const char *value, char *temp) {
void format_query(char *temp, const char *key, const char *value) {
  printf("=====prev key, value ( %s %s) temp: %s\n", key, value, temp);
  strcat(temp, " ");
  strcat(temp, key);
  strcat(temp, "=");
  strcat(temp, value);
  printf("=====next key, value ( %s %s) temp: %s\n", key, value, temp);
}

typedef struct _PrivInfo
{
    int             cond;
    onion_request   *req;
    onion_response  *res;
} PrivInfo;

static int hello(void *p, onion_request *req, onion_response *res);
static void test_thread(void *s);

/**
 * @short Just asks the user for the answer.
 */
onion_connection_status ask_handler(void *none, onion_request *req,
                                    onion_response *res)
{
    // char temp[1024];
    // strncpy(temp, onion_request_get_path(req), sizeof(temp) - 1);
    // onion_dict_preorder(onion_request_get_query_dict(req), format_query, temp);

    PrivInfo *priv = calloc(1, sizeof(PrivInfo));
    priv->req = req;
    priv->res = res;

    test_thread(priv);

    return OCS_PROCESSED;
}

/**
 * @short In a simple loop asks the user for the answer of the query.
 */
int main(int argc, char **argv) {
  onion *server;

  //server = onion_new(O_ONE_LOOP);
  server = onion_new(O_POOL);

  onion_set_hostname(server, "0.0.0.0");
  //onion_set_root_handler(server, onion_handler_new((void *)ask_handler, NULL, NULL));

  onion_url *urls = onion_root_url(server);

  onion_url_add_static(urls, "static", "Hello staticstatic world", HTTP_OK);
  //onion_url_add(urls, "timeout", random_timeout);
  //onion_url_add(urls, "", hello);
  //onion_url_add(urls, "^(.*)$", hello);
  onion_url_add(urls, "carnet/sm/lbs/hello", hello);

  ONION_INFO("Listening at http://0.0.0.0:8080");
  onion_listen(server);

  onion_free(server);

  return 0;
}

static void parameter_query(char *uri, const char *key, const char *value)
{
    printf("===== uri(%s) key, value ( %s %s)\n", uri, key, value);
}

static int hello(void *p, onion_request *req, onion_response *res)
{
    //onion_response_set_length(res, 11);

    printf("========== (%p)process url hello ==========\n", get_thread_id());
    printf("query 1: %s\n", onion_request_get_query(req, "1"));

    const char *path = onion_request_get_path(req);
onion_dict_preorder(onion_request_get_query_dict(req), parameter_query, (void*)path);
    // onion_response_write0(res, "Connection");
    // onion_response_write(res, ": ", 2);
    // onion_response_write0(res, "keep-alive");
    // onion_response_write(res, "\r\n", 2);
    // onion_response_write(res, "\r\n", 2);

    onion_response_write0(res, "Hello world");
    if (onion_request_get_query(req, "1"))
    {
        onion_response_printf(res, "<p>Path: %s",
                              onion_request_get_query(req, "1"));
    }
    onion_response_printf(res, "<p>Client description: %s",
                          onion_request_get_client_description(req));
    return OCS_PROCESSED;
}

static int service1_proc(const char *path, onion_request *req, onion_response *res)
{
    char temp[1024];
    printf("========== (%p)process for path: %s ==========\n", get_thread_id(), path);

    onion_dict_preorder(onion_request_get_query_dict(req), parameter_query, (void*)path);

    //int fd = req->connection.fd;
    snprintf(temp, sizeof(temp), "thread(%p) %s end ===", get_thread_id(), path);

    if (res)
        onion_response_write0(res, temp);

    return 0;
}

static void *test_http_service(void *arg)
{
    PrivInfo *priv = arg;
    const char *path = onion_request_get_path(priv->req);
    service1_proc(path, priv->req, priv->res);

    free(priv);

    //pthread_exit(0);
    return NULL;
}
static void test_thread(void *s)
{
    // int ret = 0;
    // pthread_t thread;

    // ret = pthread_create(&thread, NULL, test_http_service, s);
    // if (ret != 0)
    //     printf("create thread failed");

    // pthread_detach(thread);
    test_http_service(s);
}

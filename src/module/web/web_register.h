/**
 * History:
 * ================================================================
 * 2019-09-16 qing.zou created
 *
 */

#ifndef WEB_REGISTER_H
#define WEB_REGISTER_H

#include "vmp.h"
#include "api/api_demo.h"
#include "api/api_node_info.h"
#include "api/api_node_list.h"

VMP_BEGIN_DELS

int web_demo_register(void *server, void *args);
int web_node_info_register(void *server, void *args);
int web_node_list_register(void *server, void *args);

VMP_END_DELS

#endif // WEB_REGISTER_H

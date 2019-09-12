/**
 * History:
 * ================================================================
 * 2019-08-14 qing.zou created
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <sys/wait.h>

#include "smm.h"
#include "vmp.h"

#define SW_VERSION "v1.0"

enum
{
    PROCESS_APP,
    PROCESS_DAEMON,
};

#define ROOT_CFG            "./"

#define ZLOG_FILE_COUNT     5
#define ZLOG_CONF_FILE      ROOT_CFG"zlog.conf"
#define ZLOG_FILE_PATH      "/tmp/log/"

static void handler(int num)
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    VMP_LOGW("waitpid, ret = %d", pid);

    if (WIFEXITED(status))
    {
        VMP_LOGI("The child %d exit with code %d\n", pid, WEXITSTATUS(status));
    }
    signal(SIGCHLD, handler);
}

static int my_zlog_init(int procname)
{
    int rc;
    if (procname == PROCESS_APP)
        rc = dzlog_init(ZLOG_CONF_FILE, "smp");
    else
        rc = dzlog_init(ZLOG_CONF_FILE, "app");

    if (rc)
    {
        printf("zlog init failed\n");
        return -1;
    }

    VMP_LOGD("hello zlog");

    return 0;
}

int main(int argc, char **argv)
{
    int opt = 0;

    my_zlog_init(PROCESS_APP);

    fprintf(stdout, "\n****************************** ******************************\n");
    fprintf(stdout, "*             vmp-sm demo starting...                       *\n");
    fprintf(stdout, "*             version %s                                  *", SW_VERSION);
    fprintf(stdout, "\n****************************** ******************************\n\n\n");

    char *conf = NULL;
    while ((opt = getopt(argc, argv, "hvclt")) != -1)
    {
        switch (opt)
        {
        case 'h':
            //help();
            return 0;
        case 'v':
            //version();
            return 0;
        case 'c':
            conf = optarg;
            break;
        case 't':
            //type = optarg;
            break;
        case 'l':
            //showlog = 1;
        default:
            break;
        }
    }
    signal(SIGCHLD, handler);

    smm_init(argc, argv);
    smm_start();
    smm_done();

    // load_init(argc, argv);

    // load_start();

    // load_done();

    return (0);
}

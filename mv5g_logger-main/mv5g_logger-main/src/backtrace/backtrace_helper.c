#include "backtrace_helper.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#if defined (__linux__)
#include <fcntl.h>
#include <unistd.h>
#include <execinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>
#include <math.h>
#endif

struct _btrace_helper{
    char cur_app_name[32];
    char cur_app_path[32];
    char folder_path[64];
    __sighandler_t sig_func[16];
};

static void WidebrightSegvHandler(int signum) {
    void *array[10];
    size_t size;
    char **strings;
    size_t i, j;

    signal(signum, SIG_DFL); /* 还原默认的信号处理handler */

    size = backtrace(array, 10);
    strings = (char **)backtrace_symbols(array, size);

    fprintf(stderr, "crash signal received! Stack trace:\n");
    for (unsigned i = 0; i < size; i++) {
        fprintf(stderr, "%d %s \n", i, strings[i]);
    }

    free (strings);
    exit(1);
}

btrace_helper* create_btrace_helper() {
    btrace_helper* helper = (btrace_helper*)malloc(sizeof(btrace_helper));
    if(NULL == helper) return NULL;
    memset(helper, 0, sizeof(helper));

    return helper;
}

BTRACE_ERRORCODE destroy_btrace_helper(btrace_helper** obj) {
    BTRACE_ERRORCODE rc = kBtraceSuccess;
    if(NULL == obj || NULL == *obj) {
        rc = kBtraceObjIsNull;
        return rc;
    }

    free(*obj);
    obj = NULL;
    return rc;
}

#if defined (__linux__)
BTRACE_ERRORCODE create_log_dir(btrace_helper* obj, const char* dir_name) {
    BTRACE_ERRORCODE rc = kBtraceSuccess;
    if(NULL == obj) {
        rc = kBtraceObjIsNull;
        return rc;
    }

    return rc;
}

BTRACE_ERRORCODE set_log_dir(btrace_helper* obj, const char* dir_name) {
    BTRACE_ERRORCODE rc = kBtraceSuccess;
    if(NULL == obj) {
        rc = kBtraceObjIsNull;
        return rc;
    }

    return rc;
}

BTRACE_ERRORCODE set_signal_callback(btrace_helper* obj, int sig, __sighandler_t cb) {
    BTRACE_ERRORCODE rc = kBtraceSuccess;
    if(NULL == obj) {
        rc = kBtraceObjIsNull;
        return rc;
    }

    obj->sig_func[sig] = cb;
    return rc;
}

BTRACE_ERRORCODE register_exception_handler(btrace_helper* obj, int auto_process) {
    BTRACE_ERRORCODE rc = kBtraceSuccess;
    if(NULL == obj) {
        rc = kBtraceObjIsNull;
        return rc;
    }

    for(unsigned sig_num = 0; sig_num < 16; sig_num++) {
        if(NULL != obj->sig_func[sig_num]) 
            signal(sig_num, obj->sig_func[sig_num]);
    }

    if(auto_process) {
        if(NULL == obj->sig_func[SIGBUS])
            signal(SIGBUS, WidebrightSegvHandler);
        
        if(NULL == obj->sig_func[SIGSEGV])
            signal(SIGSEGV, WidebrightSegvHandler);

        if(NULL == obj->sig_func[SIGABRT])
            signal(SIGABRT, WidebrightSegvHandler);
    }
    return rc;
}
#endif
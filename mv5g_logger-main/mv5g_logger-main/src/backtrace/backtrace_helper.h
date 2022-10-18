#ifndef __BACKTRACE_HELPER_H__
#define __BACKTRACE_HELPER_H__
#include <signal.h>

struct _btrace_helper;
typedef struct _btrace_helper btrace_helper;

typedef enum _BTRACE_ERRORCODE {
    kBtraceSuccess = 0,
    kBtraceError   = -1,
    kBtraceObjIsNull = -2,
    kBtraceFreeObjFail = -3,
    kBtraceErrorRegisterHandlerError = -4
}BTRACE_ERRORCODE;

btrace_helper* create_btrace_helper();
/**
* @brief free the memory
* 
* @author zzk
* @date 2022-08-17 15:34:37
* @version 
* @param obj 
* 
* @return kBtraceSuccess; kBtraceObjIsNull; kBtraceFreeObjFail
*/
BTRACE_ERRORCODE destroy_btrace_helper(btrace_helper** obj);
#if defined (__linux__)
BTRACE_ERRORCODE set_app_name(btrace_helper* obj, const char* app_name);
BTRACE_ERRORCODE create_log_dir(btrace_helper* obj, const char* dir_name);
BTRACE_ERRORCODE set_log_dir(btrace_helper* obj, const char* dir_name);

/**
* @brief if you need to process the signal by your own function, call this func 
         to register the callback.
* 
* @author zzk
* @date 2022-08-18 10:34:16
* @version 
* @param obj
* @param sig 
* @param cb
*
* @return kBtraceSuccess; kBtraceObjIsNull
*/
BTRACE_ERRORCODE set_signal_callback(btrace_helper* obj, int sig, __sighandler_t cb);
/**
* @brief
* 
* @author zzk
* @date 2022-08-17 15:38:00
* @version 
* @param obj
* @param auto_process Judge whether auto register the process of SIGBUS、SIGSEGV、SIGABRT.
*                     When the value is zero, means do not auto process, otherwise means auto process.
* 
* @return kBtraceSuccess; kBtraceObjIsNull; kBtraceErrorRegisterHandlerError
*/
BTRACE_ERRORCODE register_exception_handler(btrace_helper* obj, int auto_process);
#endif

#endif

#include <signal.h>
#include <unistd.h>
// #include "mv_ipc_operator.h"
#include "backtrace/backtrace_helper.h"

static int g_exit = 0;

static void sigroutine(int signo) { 
	switch (signo) {
		case SIGHUP:
			printf("Get a signal -- SIGHUP \n");
			g_exit = 1;
		break;
		case SIGINT:
			printf("Get a signal -- SIGINT \n");
			g_exit = 1;
		break;
		case SIGQUIT:
			printf("Get a signal -- SIGQUIT \n");
			g_exit = 1;
		break;
		case SIGSEGV:
			printf("Get a signal -- SIGSEGV \n");
			g_exit = 1;
		break;
		case SIGTERM:
			printf("Get a signal -- SIGTERM \n");
			g_exit = 1;
		break;
	}
}

void func() {
	int* p = NULL;
	
	static int a = 0;
	sleep(1);
	a++;
	if(15 == a) *p = 1;
}

int main(int argc, char* argv[]) {
    btrace_helper* helper = create_btrace_helper();
	if(NULL == helper) g_exit = 1;

	set_signal_callback(helper, SIGINT, sigroutine); 
	
	register_exception_handler(helper, 1);

    while(!g_exit) {
		func();
    }

	destroy_btrace_helper(&helper);
    return 0;
}
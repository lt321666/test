#include <stdio.h>   
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

long long get_sys_time_ms(void)
{
    long long time_ms = 0;
    struct timeval sys_current_time;

    gettimeofday(&sys_current_time, NULL);
    time_ms = ((long long)sys_current_time.tv_sec*1000000 + sys_current_time.tv_usec) / 1000;

    return time_ms;
}

int main(int arc, char *argv[])
{
 long long cur_sys_time = get_sys_time_ms();

    printf("cur_sys_time = %lld ms\n", cur_sys_time);

 return 0;
}
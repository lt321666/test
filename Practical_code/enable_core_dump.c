#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#define SHELL_CMD_CONF_CORE_FILE    "echo /var/core-%e-%p-%t > /proc/sys/kernel/core_pattern"
#define SHELL_CMD_DEL_CORE_FILE     "rm -f /var/core*"

static int enable_core_dump(void)
{
    int ret = -1;
    int resource = RLIMIT_CORE;
    struct rlimit rlim;

    rlim.rlim_cur = 1 ? RLIM_INFINITY : 0;
    rlim.rlim_max = 1 ? RLIM_INFINITY : 0;

    system(SHELL_CMD_DEL_CORE_FILE);

    if (0 != setrlimit(resource, &rlim))
    {
        printf("setrlimit error!\n");
        return -1;
    }
    else
    {
        system(SHELL_CMD_CONF_CORE_FILE);
        printf("SHELL_CMD_CONF_CORE_FILE\n");
        return 0;
    }

    return ret;
}

int main(int argc, char **argv)
{
    enable_core_dump();

    printf("==================segmentation fault test==================\n");

    int *p = NULL;
    *p = 1234;

    return 0;
}
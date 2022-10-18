#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int get_local_ip(const char *_ifr_name, char *_ip)
{
 int ret = -1;
    int sockfd;
    struct sockaddr_in sin;
    struct ifreq ifr;
 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sockfd)
    {
        printf("socket error\n");
        return ret;
    }
 
    strncpy(ifr.ifr_name, _ifr_name, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;
 
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0)
    {
        printf("ioctl error\n");
        close(sockfd);
        return ret;
    }
 
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    int ip_len = snprintf(_ip, 32, "%s", inet_ntoa(sin.sin_addr));
 
    close(sockfd);
 ret = ip_len;

 return ret;
}

int main(int argc, char **argv)
{
    char ip_str[32] = {0};
    get_local_ip("wlan1", ip_str);
    printf("ip = %s\n", ip_str);

    return 0;
}
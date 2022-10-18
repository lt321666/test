#include <sys/stat.h>  
#include <unistd.h>  
#include <stdio.h>  

long get_file_size(const char *_file_name)
{
    FILE * fp = fopen(_file_name, "r");
    if (NULL == fp)
    {
        printf("fopen error\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    return size;
}

int main()
{
    #define FILE_NAME  "./get_file_size"
    long file_size = get_file_size(FILE_NAME);
    printf("file_size = %ld\n", file_size);

    return 0;
}
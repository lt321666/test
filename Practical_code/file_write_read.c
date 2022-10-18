#include <stdio.h>   

static int file_opt_write(const char *filename, void *ptr, int size)
{   
    FILE *fp;
    size_t num;

    fp = fopen(filename, "wb");
    if(NULL == fp)
    {
        printf("open %s file error!\n", filename);
        return -1;   
    }
    
    num = fwrite(ptr, 1, size, fp);
    if(num != size)
    {
        fclose(fp);
        printf("write %s file error!\n", filename);
        return -1;
    } 

    fclose(fp);

    return num;
}

static int file_opt_read(const char *filename, void *ptr, int size)
{
    FILE *fp;
    size_t num;

    fp = fopen(filename, "rb");
    if(NULL == fp)
    {
        printf("open %s file error!\n", filename);
        return -1;
    }
    
    num = fread(ptr, 1, size, fp);
    if(num != size)
    {
        fclose(fp);
        printf("write %s file error!\n", filename);
        
        return -1;
    } 
    fclose(fp);

    return num;
}

typedef struct _test_struct
{
 char a;  
 char c; 
 short b;         
 int d;
}test_struct;

int main(int arc, char *argv[])
{
    #define FILE_NAME  "./test_file"

    test_struct write_data = {0};
    write_data.a = 1;
    write_data.b = 2;
    write_data.c = 3;
    write_data.d = 4;
    printf("write_data.a = %d\n", write_data.a);
    printf("write_data.b = %d\n", write_data.b);
    printf("write_data.c = %d\n", write_data.c);
    printf("write_data.d = %d\n", write_data.d);
    file_opt_write(FILE_NAME, (test_struct*)&write_data, sizeof(test_struct));

    test_struct read_data = {0};
    file_opt_read(FILE_NAME, (test_struct*)&read_data, sizeof(test_struct));
    printf("read_data.a = %d\n", read_data.a);
    printf("read_data.b = %d\n", read_data.b);
    printf("read_data.c = %d\n", read_data.c);
    printf("read_data.d = %d\n", read_data.d);

 return 0;
}
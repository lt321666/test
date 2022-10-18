#include <stdio.h>   

// 获取结构体成员大小
#define  GET_MEMBER_SIZE(type, member)   sizeof(((type*)0)->member)

// 获取结构体成员偏移量
#define  GET_MEMBER_OFFSET(type, member)  ((size_t)(&(((type*)0)->member)))

typedef struct _test_struct0
{
 char x;  
 char y; 
 char z;
}test_struct0;

typedef struct _test_struct1
{
 char a;  
 char c; 
 short b;         
 int d;
 test_struct0 e;
}test_struct1;

int main(int arc, char *argv[])
{
 printf("GET_MEMBER_SIZE(test_struct1, a) = %ld\n", GET_MEMBER_SIZE(test_struct1, a));
    printf("GET_MEMBER_SIZE(test_struct1, c) = %ld\n", GET_MEMBER_SIZE(test_struct1, c));
 printf("GET_MEMBER_SIZE(test_struct1, b) = %ld\n", GET_MEMBER_SIZE(test_struct1, b));
 printf("GET_MEMBER_SIZE(test_struct1, d) = %ld\n", GET_MEMBER_SIZE(test_struct1, d));
    printf("GET_MEMBER_SIZE(test_struct1, e) = %ld\n", GET_MEMBER_SIZE(test_struct1, e));
    printf("test_struct1 size = %ld\n", sizeof(test_struct1));

 printf("GET_MEMBER_OFFSET(a): %ld\n", GET_MEMBER_OFFSET(test_struct1, a));
 printf("GET_MEMBER_OFFSET(c): %ld\n", GET_MEMBER_OFFSET(test_struct1, c));
 printf("GET_MEMBER_OFFSET(b): %ld\n", GET_MEMBER_OFFSET(test_struct1, b));
 printf("GET_MEMBER_OFFSET(d): %ld\n", GET_MEMBER_OFFSET(test_struct1, d));
 printf("GET_MEMBER_OFFSET(e): %ld\n", GET_MEMBER_OFFSET(test_struct1, e));

 return 0;
}
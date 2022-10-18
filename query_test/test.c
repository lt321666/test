#include<stdio.h>
void mian(){
typedef struct _eg
{
     int x;
     int y;
}eg;
int point = 0x30000000;
eg *peg = (eg*)point;
printf("add = %p ,value1_add = %d ,value2_add = %d ，value1 = %d ,value2 = %d\n",peg,peg->x,peg->y,(*peg).x,(*peg).y);
//eg ptg = (eg*)point;
//printf("add = %p ,value1_add = %p ,value2_add = %p ，value1 = %d ,value2 = %d\n",ptg,ptg.x,ptg.y,ptg->x,ptg->y);

}

//验证两件事1、结构体指针赋值和结构体赋值，2、指针偏移量
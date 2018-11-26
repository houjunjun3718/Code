/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月09日 星期二 14时48分48秒
 ************************************************************************/

#include<stdio.h>

#include"Transmission.h"

int main()
{
    int ren;
    ren = getVersion();
    printf("ren = %d\n",ren);
    ren = pushData();
    printf("ren = %d\n",ren);

    return 0;
}




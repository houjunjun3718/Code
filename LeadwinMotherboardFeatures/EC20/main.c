/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月10日 星期一 13时47分44秒
 ************************************************************************/

#include<stdio.h>


#include"EC20.h"


int main(void)
{
    int rc;
    rc = EC20_Restart();
    printf("rc = %d\n",rc);

    return 0;
}




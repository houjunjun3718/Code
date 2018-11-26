/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月12日 星期一 15时05分07秒
 ************************************************************************/

#include<stdio.h>

#include"Reteiever_Processing.h"

int main(void)
{
    //待解析的数据
    char data[200] = {"RS;2018-08-27 16:55;10180019;100;20180060;40;100;4;9;A;0;3;a;73.8;A;1;37;a;6.6;A;2;38;a;0.10;B;0;3;a;73.9;B;1;37;a;8.1;B;2;38;a;0.06;C;0;3;a;-----;C;1;37;a;-----;C;2;38;a;-----;26D4"};

    PUP pup_data;
    Reteiever_Processing(data,&pup_data);
    printf("时间:%s\n",pup_data.time);
    
    return 0;
}





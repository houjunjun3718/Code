/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年12月07日 星期五 21时23分31秒
 ************************************************************************/

#include<stdio.h>

#include"LEDDisplayNetworkPortModule.h"


int main(void)
{
    
    char buff0[20] = {"20180063"};
    char buff1[20] = {"12-07 11:35"};
    char buff2[20] = {"07.4"};
    char buff3[20] = {"30.5"};
    char buff4[20] = {"0.47"};
    char buff5[20] = {"08.7"};
    char buff6[20] = {"25.4"};
    char buff7[20] = {"1.32"};
    
    char *data[10] = {0};
    data[0] = buff0;
    data[1] = buff1;
    data[2] = buff2;
    data[3] = buff3;
    data[4] = buff4;
    data[5] = buff5;
    data[6] = buff6;
    data[7] = buff7;
    
    if(0 > InitLEDModule("192.168.0.236",5005))
    {
        printf("InitLEDModule:error!!!\n");
        return -1;
    }
    printf("InitLEDModule:OK!!!\n");

    if(0 > SendLEDData(data,8))
    {
        printf("SendLEDData:error!!!\n");
    }
    printf("SendLEDData:OK!!!\n");
  
    while(1);
    return 0;
}








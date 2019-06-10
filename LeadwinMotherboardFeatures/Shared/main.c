/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年05月14日 星期二 16时42分47秒
 ************************************************************************/

#include<stdio.h>


#include"Shared.h"


int main(void)
{
   unsigned char request[8] = {0x02,0x03,0x00,0x12,0x00,0x03,0x00,0x00};
    unsigned int num = 6;
    int crc16 = 0;
    crc16 = calc_crc16(request,num);
    request[6] = crc16 % 256;
    request[7] = crc16 / 256;

    printf("%x\n",crc16);

    for(int i = 0;i < 8;i++)
    {
        printf("%x,",request[i]);
    }
    printf("\n");
    return 0;
}





/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月18日 星期四 09时44分05秒
 ************************************************************************/

#include<stdio.h>

#include"Data_Delivery_Cloud.h"


#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"

int main(void)
{
    init_Data_Delivery_Cloud(IP);

//    char data[200]={"RD;2018-07-18 14:47;10180019;DH;01.06.10;0000;0000;0013A200410ACCC0;085;0;2;08.34;02.40;08.00;S;0;0BB8;7404;1689"};
//    char data[200]={"RS;2018-11-14 09:54;10180019;100;20180060;40;028;4;6;A;0;3;a;61.5;A;1;37;a;19.5;A;2;38;a;0.10;B;0;3;a;61.6;B;1;37;a;6.2;B;2;38;a;0.01;1D4C"};
//    char data[200] = {"RT;time error"};
    char data[200] = {"RE;2018-11-16 20:00:00;10180019;error;The retriever device is incorrect and has not received data for a long time."};
    comply_Data_Delivery_Cloud(DATA_IP,data);

    exit_Data_Delivery_Cloud();

    return 0;
}

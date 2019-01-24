/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月18日 星期四 09时44分05秒
 ************************************************************************/

#include<stdio.h>

#include"Data_Delivery_Cloud.h"


#define IP "http://101.201.80.90/LWI-getVersion.php"
#define DATA_IP "http://101.201.80.90/LWI-pushData.php"

int main(void)
{
    init_Data_Delivery_Cloud(IP);

//    char data[200]={"RD;2018-07-18 14:47;10180019;DH;01.06.10;0000;0000;0013A200410ACCC0;085;0;2;08.34;02.40;08.00;S;0;0BB8;7404;1689"};char data[200]={"RS;2018-11-14 :55;10180016;100;13446;40;080;4;3;A;0;3;a;082.9;A;1;37;a;008.5;A;2;38;a;0321.;1D4C"};
//    char data[200]={"RS;2019-01-01 09:56;20180016;---;247513446;40;080;4;3;A;0;3;a;082.9;A;1;37;a;008.1;A;2;38;a;0300.;1D4C"};
//    char data[200] = {"RE;2018-11-16 20:00:00;10180019;error;The retriever device is incorrect and has not received data for a long time."};
    char data[200] = {"RS;2018-12-26 08:00;10180012;081;20180051;55;066;4;6;A;0;3;a;50.7;A;1;37;a;30.9;A;2;38;a;0.13;B;0;3;a;54.8;B;1;37;a;23.7;B;2;38;a;0.03;1D90"};
    comply_Data_Delivery_Cloud(DATA_IP,data);

    exit_Data_Delivery_Cloud();

    return 0;
}

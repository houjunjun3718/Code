/*************************************************************************
	> File Name: ceshi.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月11日 星期四 13时26分36秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<time.h>

#include"Data_Delivery_Cloud.h"

#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"

int main()
{
    
    time_t t;
    struct tm * tb;
    char times[1024];
    while(init_Data_Delivery_Cloud(IP) < 0)
    {
        sleep(1);
    }
    while(1)
    {
        printf("1111\n");
        time(&t);
        tb = localtime(&t);
        sprintf(times,"RS;%4d-%02d-%02d %02d:%02d:%02d;10180023;099;20180083;40;099;4;6;A;0;3;a;99.9;A;1;37;a;99.9;A;2;38;a;9.99;B;0;3;a;99.9;B;1;37;a;99.9;B;2;38;a;9.99;1D2C",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday,tb->tm_hour,tb->tm_min,tb->tm_sec);

        comply_Data_Delivery_Cloud(DATA_IP,times);
        printf("%s\n",times);
        sleep(300);

    }



    return 0;
}

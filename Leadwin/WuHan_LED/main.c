/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月26日 星期三 15时20分43秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<string.h>

#include"LED.h"

int main(void)
{
    switch_1 = 0;
    pthread_t pthread_LED;
    pthread_create(&pthread_LED,NULL,(void *)write_LED,NULL);
    pthread_detach(pthread_LED);
a:  if(switch_1 == 0)
    {
        strcpy(led_data[0].id,"20180060");
        strcpy(led_data[1].id,"20180070");

        strcpy(led_data[0].A_temp,"32.7");
        strcpy(led_data[1].A_temp,"25.4");
        strcpy(led_data[0].B_temp,"13.6");
        strcpy(led_data[1].B_temp,"5.6");

        strcpy(led_data[0].A_homidity,"15.3");
        strcpy(led_data[1].A_homidity,"12.5");
        strcpy(led_data[0].B_homidity,"9.4");
        strcpy(led_data[1].B_homidity,"3.2");

        strcpy(led_data[0].A_EC,"0.04");
        strcpy(led_data[1].A_EC,"0.13");
        strcpy(led_data[0].B_EC,"0.47");
        strcpy(led_data[1].B_EC,"0.16");
        switch_1 = 1;
        printf("1111\n");
        
    }else{
        goto a;
    }


/*
    led_data[0].id = "20180060";
    led_data[1].id = "20180070";
    
    led_data[0].A_temp = "32.7";
    led_data[1].A_temp = "25.4";
    led_data[0].B_temp = "33.4";
    led_data[1].B_temp = "22.5";
    
    led_data[0].A_homidity = "15.3";
    led_data[1].A_homidity = "12.5";
    led_data[0].B_homidity = "9.8";
    led_data[1].B_homidity = "13.7";
    
    led_data[0].A_EC = "0.04";
    led_data[1].A_EC = "0.13";
    led_data[0].B_EC = "0.47";
    led_data[1].B_EC = "0.16";
*/
    

    while(1);
    return 0;
}



/*************************************************************************
	> File Name: LED.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年09月22日 星期六 11时08分13秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<strings.h>
#include<arpa/inet.h>
#include<unistd.h>

#include"LED.h"


/*********************************************
 * 初始化ＬＥＤ显示屏
 * 参数１：ＬＥＤ显示屏的ｉｐ地址
 * 参数２：ＬＥＤ显示屏的端口
 * 返回值：ＬＥＤ显示屏的ＴＣＰ套接字
 * ******************************************/
int initLED(char *ip,int port)
{
    int sockfd; 
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    return sockfd;
}


/*********************************************
 * 向ＬＥＤ显示屏发送数据的线程
 * 无参数
 * 无返回值
 * ******************************************/
void *write_LED(void)
{
    char data_A[1024] = {"!#001%1    北京领旋智慧农业    \r\n一区    浅位    深位\r\n温度℃　 ----    ----\r\n湿度％  ----    ----\r\nECmS/cm ----    ----$$"};
    char data_B[1024] = {"!#001%1    北京领旋智慧农业    \r\n二区    浅位    深位\r\n温度℃　 ----    ----\r\n湿度％  ----    ----\r\nECmS/cm ----    ----$$"};
    int ren = 0;
    int sun = 0;
    double su = 32.7;
    printf("%s\n",data_A); 
    printf("%s\n",data_B);

    while(1)
    {  
//        printf("%s\n",data_A); 
#if 0
        for(int i = 88;i<102;i++)
        {
            printf("%c",data_A[i]);
        }

        printf("\n");
endif
        sprintf(data_B+82,"%2.1f",su);
        data_B[86] = ' ';
        sprintf(data_B+90,"%2.1f",su);
        data_B[94] = ' ';
        sprintf(data_B+107,"%2.1f",su);
        data_B[111] = ' ';
        sprintf(data_B+115,"%2.1f",su);
        data_B[119] = ' ';
        sprintf(data_B+129,"%2.1f",su);
        data_B[133] = ' ';
        sprintf(data_B+137,"%2.1f",su);
        data_B[141] = '$';
#endif        
//        printf("%s\n",data_B);
        

        if(switch_1 == 1){
            switch_1 = 0;
            printf("id1: %s\n",led_data[0].id);
            if(strcmp(led_data[0].id,"20180060") == 0)
            {
                memcpy(data_A+82,led_data[0].A_temp,strlen(led_data[0].A_temp));
                if(strlen(led_data[0].A_temp) == 3)
                    data_A[85] = '0';
                memcpy(data_A+90,led_data[0].B_temp,strlen(led_data[0].B_temp));
                if(strlen(led_data[0].B_temp) == 3)
                    data_A[93] = '0';
                memcpy(data_A+107,led_data[0].A_homidity,strlen(led_data[0].A_homidity));
                if(strlen(led_data[0].A_homidity) == 3)
                    data_A[110] = '0';
                memcpy(data_A+115,led_data[0].B_homidity,strlen(led_data[0].B_homidity));
                if(strlen(led_data[0].B_homidity) == 3)
                    data_A[118] = '0';
                memcpy(data_A+129,led_data[0].A_EC,strlen(led_data[0].A_EC));
                if(strlen(led_data[0].A_EC) == 3)
                    data_A[132] = '0';
                memcpy(data_A+137,led_data[0].B_EC,strlen(led_data[0].B_EC));
                if(strlen(led_data[0].B_EC) == 3)
                    data_A[140] = '0';
            }else 
            {
                memcpy(data_B+82,led_data[0].A_temp,strlen(led_data[0].A_temp));
                if(strlen(led_data[0].A_temp) == 3)
                    data_B[85] = '0';
                memcpy(data_B+90,led_data[0].B_temp,strlen(led_data[0].B_temp));
                if(strlen(led_data[0].B_temp) == 3)
                    data_B[93] = '0';
                memcpy(data_B+107,led_data[0].A_homidity,strlen(led_data[0].A_homidity));
                if(strlen(led_data[0].A_homidity) == 3)
                    data_B[110] = '0';
                memcpy(data_B+115,led_data[0].B_homidity,strlen(led_data[0].B_homidity));
                if(strlen(led_data[0].B_homidity) == 3)
                    data_B[118] = '0';
                memcpy(data_B+129,led_data[0].A_EC,strlen(led_data[0].A_EC));
                if(strlen(led_data[0].A_EC) == 3)
                    data_B[132] = '0';
                memcpy(data_B+137,led_data[0].B_EC,strlen(led_data[0].B_EC));
                if(strlen(led_data[0].B_EC) == 3)
                    data_B[140] = '0';
                
            }
            if(strcmp(led_data[1].id,"20180060") == 0)
            {
                memcpy(data_A+82,led_data[1].A_temp,strlen(led_data[1].A_temp));
                if(strlen(led_data[1].A_temp) == 3)
                    data_A[85] = '0';
                memcpy(data_A+90,led_data[1].B_temp,strlen(led_data[1].B_temp));
                if(strlen(led_data[1].B_temp) == 3)
                    data_A[93] = '0';
                memcpy(data_A+107,led_data[1].A_homidity,strlen(led_data[1].A_homidity));
                if(strlen(led_data[1].A_homidity) == 3)
                    data_A[110] = '0';
                memcpy(data_A+115,led_data[1].B_homidity,strlen(led_data[1].B_homidity));
                if(strlen(led_data[1].B_homidity) == 3)
                    data_A[118] = '0';
                memcpy(data_A+129,led_data[1].A_EC,strlen(led_data[1].A_EC));
                if(strlen(led_data[1].A_EC) == 3)
                    data_A[132] = '0';
                memcpy(data_A+137,led_data[1].B_EC,strlen(led_data[1].B_EC));
                if(strlen(led_data[1].B_EC) == 3)
                    data_A[140] = '0';
            }else 
            {
                memcpy(data_B+82,led_data[1].A_temp,strlen(led_data[1].A_temp));
                if(strlen(led_data[1].A_temp) == 3)
                    data_B[85] = '0';
                memcpy(data_B+90,led_data[1].B_temp,strlen(led_data[1].B_temp));
                if(strlen(led_data[1].B_temp) == 3)
                    data_B[93] = '0';
                memcpy(data_B+107,led_data[1].A_homidity,strlen(led_data[1].A_homidity));
                if(strlen(led_data[1].A_homidity) == 3)
                    data_B[110] = '0';
                memcpy(data_B+115,led_data[1].B_homidity,strlen(led_data[1].B_homidity));
                if(strlen(led_data[1].B_homidity) == 3)
                    data_B[118] = '0';
                memcpy(data_B+129,led_data[1].A_EC,strlen(led_data[1].A_EC));
                if(strlen(led_data[1].A_EC) == 3)
                    data_B[132] = '0';
                memcpy(data_B+137,led_data[1].B_EC,strlen(led_data[1].B_EC));
                if(strlen(led_data[1].B_EC) == 3)
                    data_B[140] = '0';
                
            }
            switch_1 = 1;
            printf("id2: %s\n",led_data[0].id);
            
            printf("%s\n",data_A);
            sleep(30);
            printf("%s\n",data_B);
            sleep(30);
        }else{
            continue;
        }
    }
}

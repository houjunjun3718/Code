/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年03月27日 星期三 13时54分07秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<time.h>

#include"IOT_Communication.h"


int main(void)
{
    void *pclient = NULL;
    pclient = Leadwin_IOT_Communicaton_Init();
    if(pclient == NULL)
    {
        printf("初始化失败\n");
        return -1;
    }

    if(0 != Leadwin_IOT_Get(pclient,200))
    {
        printf("订阅失败\n");
        return -1;
    }

    char updata[1024] = {0};
    time_t t;
//    struct tm *tb;
    time(&t);
//    tb = localtime(&t);

    sprintf(updata,"{\"querID\":\"00000001\",\"time\":\"%ld\",\"Number_of_data\":\"12\",\"0\":\"10180016;DL;30;%ld\",\"0_characteristic\":\"stat\",\"1\":\"20180060;DL;99;%ld\",\"1_characteristic\":\"stat\",\"2\":\"20180060;XH;40;%ld\",\"2_characteristic\":\"stat\",\"3\":\"20180060A;03;75.4;%ld\",\"3_characteristic\":\"data\",\"4\":\"20180060A;36;36.3;%ld\",\"4_characteristic\":\"data\",\"5\":\"20180060A;37;0.35;%ld\",\"5_characteristic\":\"data\",\"6\":\"20180060B;03;70.4;%ld\",\"6_characteristic\":\"data\",\"7\":\"20180060B;36;35.3;%ld\",\"7_characteristic\":\"data\",\"8\":\"20180060B;37;0.17;%ld\",\"8_characteristic\":\"data\",\"9\":\"20180060C;03;72.0;%ld\",\"9_characteristic\":\"data\",\"A\":\"20180060C;36;20.0;%ld\",\"A_characteristic\":\"data\",\"B\":\"20180060C;37;0.05;%ld\",\"B_characteristic\":\"data\"}",t,t,t,t,t,t,t,t,t,t,t,t,t); 

    if(0 != Leadwin_IOT_Update(pclient,updata))
    {
        printf("数据发送失败\n");
        return -1;
    }

    while(1)
    {
        time(&t);
//        tb = localtime(&t);
    sprintf(updata,"{\"querID\":\"00000001\",\"time\":\"%ld\",\"Number_of_data\":\"12\",\"0\":\"10180016;DL;30;%ld\",\"0_characteristic\":\"stat\",\"1\":\"20180060;DL;99;%ld\",\"1_characteristic\":\"stat\",\"2\":\"20180060;XH;40;%ld\",\"2_characteristic\":\"stat\",\"3\":\"20180060A;03;75.4;%ld\",\"3_characteristic\":\"data\",\"4\":\"20180060A;36;36.3;%ld\",\"4_characteristic\":\"data\",\"5\":\"20180060A;37;0.35;%ld\",\"5_characteristic\":\"data\",\"6\":\"20180060B;03;70.4;%ld\",\"6_characteristic\":\"data\",\"7\":\"20180060B;36;35.3;%ld\",\"7_characteristic\":\"data\",\"8\":\"20180060B;37;0.17;%ld\",\"8_characteristic\":\"data\",\"9\":\"20180060C;03;72.0;%ld\",\"9_characteristic\":\"data\",\"A\":\"20180060C;36;20.0;%ld\",\"A_characteristic\":\"data\",\"B\":\"20180060C;37;0.05;%ld\",\"B_characteristic\":\"data\"}",t,t,t,t,t,t,t,t,t,t,t,t,t); 
//        sprintf(updata,"{\"Number_of_data\":\"11\",\"0\":\"10180016;DL;30;2019-03-25 15:30\",\"1\":\"20180060;DL;99;2019-02-25 15:30\",\"2\":\"20180060;XH;40;2019-03-23 15:30\",\"3\":\"20180060A;03;75.4;2019-02-23 15:30\",\"4\":\"20180060A;36;36.3;2019-03-25 15:30\",\"5\":\"20180060A;37;0.35;2019-03-25 15:30\",\"6\":\"20180060B;03;70.4;2019-02-25 15:30\",\"7\":\"20180060B;36;35.3;2019-03-25 15:30\",\"8\":\"20180060B;37;0.17;2019-03-25 15:30\",\"9\":\"20180060C;03;72.0;2019-03-25 15:30\",\"A\":\"20180060C;36;20.0;2019-03-25 15:30\",\"B\":\"20180060C;37;0.05;%4d-%02d-%02d %02d:%02d:%02d\"}",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday,tb->tm_hour,tb->tm_min,tb->tm_sec); 
        if(0 != Leadwin_IOT_Update(pclient,updata))
        {
            printf("数据发送失败\n");
            return -1;
        }
        sleep(300);

    }
    

    return 0;
}





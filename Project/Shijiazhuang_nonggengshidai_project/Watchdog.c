/*************************************************************************
    > File Name: Watchdog.c
    > Author: HJJ
    > Mail: 15847703113@163.com
    > Created Time: 2018年11月13日 星期二 09时54分33秒
#define DATA_IP "http://101.201.80.90/LWI-pushData.php" 
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#include"Watchdog.h"
#include"curl/curl.h"
#include"Data_Delivery_Cloud.h"


//定义一把锁,
char retriever_signal;

static int Time;

/*************************************************************************
 * 超时的处理函数,向服务器发送超时信息
 * 参数:无参数
 * 返回值:成功返回0
 *        失败返回-1
 * **********************************************************************/
int TimeoutHandler(void)
{
    CURL *curl;
    CURLcode res;
    time_t t = time(0);
    char tmp[64];
    strftime(tmp,sizeof(tmp),"%Y-%m-%d %X",localtime(&t));
    char postthis[200];
    sprintf(postthis,"RE;%s;10180083;error;The retriever device is incorrect and has not received data for a long time.",tmp);

    
    //调用数据发送函数
    comply_Data_Delivery_Cloud("http://cloud.leadwin-agritech.com/LWI-pushData.php",postthis);
    printf("%s",postthis);    
    return 0;
}


/*************************************************************************
 * 看门狗线程
 * 无参数
 * 无返回值
 * **********************************************************************/
void *watchdog(void)
{
    int i;
    while(1)
    {
        for(i = 0;i < Time;i++)
        {
            printf("i = %d\n",i);
            printf("Time = %d\n",Time);
            printf("retriever_signal = %d\n",retriever_signal);
            if(retriever_signal == 0)//判断条件为喂狗的全局变量
            {
                i = 0;
                retriever_signal = 1;
                printf("retriever_signal:i = %d\n",i);
            }
            sleep(60);
        }
        //时间到了,执行超时函数,
        printf("超时,,,\n");
        TimeoutHandler();
        i = 0;

    }
}



/*************************************************************************
 * 初始化看门狗,开启计时
 * 参数1:时间(单位:s)
 * 参数2:要监视的设备
 * 返回值:成功返回0
 *        失败返回-1
 * **********************************************************************/
int initWatchdog(int time,int device)
{
    //识别要监控的设备,启动看门狗的线程
    if(SpectrumRetriever == device)
    {
        retriever_signal = 1;   //0标示有数据.
        
        //创建看门狗的线程
        pthread_t pthread_fd;
        Time = time;
//        printf("11111111\n");
        pthread_create(&pthread_fd,NULL,(void *)watchdog,NULL);
        pthread_detach(pthread_fd);
        
        printf("创建看门狗\n");
        return 0;

    }else{
        return -1;
    }
    

}

/*************************************************************************
 * 喂狗函数
 * 参数1:要监视的设备
 * 返回值:成功返回0
 *        失败返回-1
 * **********************************************************************/
int loadWatchdog(int device)
{
    if(SpectrumRetriever == device)
    {
        printf("loadWatchdog:retriever_signal = %d\n",retriever_signal);
        retriever_signal = 0;
        printf("喂狗retriever_signal = %d\n",retriever_signal);
        return 0;
    }else{
        printf("输入的参数错误\n");
        return -1;
    }
}





/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年12月04日 星期二 13时25分59秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sqlite3.h>
#include<pthread.h>

#include"Data_Delivery_Cloud.h"
#include"Watchdog.h"
#include"ReuseFunction.h"
#include"Reteiever_Processing.h"
#include"Retriever_collection.h"
#include"LEDDisplayNetworkPortModule.h"
#include "Shared.h"
#include "MEC10_Sensor.h"
#include "Log.h"


#define MECTTY "/dev/ttyO3"  //1号485串口的设备号

#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"

#define TIME 20

#define RETRIEVER "10198665"  //Retriever的SN
#define PUP "20282301"  //Pup的SN

#define LOG "/home/root/log.txt"

sqlite3 *db;   //数据库
pthread_mutex_t lock;    //数据库操作锁

//发送数据 
int callback_song(void *NotUsed,int argc,char **argv,char **azColName)
{
    int i = 0;
    for(;i < argc;i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i] : "NULL");
    }
    int a = 0;
    a = comply_Data_Delivery_Cloud(DATA_IP,argv[0]);  //发送数据到云平台
    if(a < 0)
    {
        WriteLog("数据上传失败!\n");
        printf("数据发送失败\n");
        return -1;
    }
    WriteLog(argv[0]);
    return 0;
}

int callback(void *NotUsed,int argc,char **argv,char **azColName)
{
    int i = 0;
    for(;i < argc;i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i] : "NULL");
    }

    long int ren = 0;
    //数据发送,将数据发送到服务器,如果数据发送失败就重新发送
    char mommand[1024];
    sprintf(mommand,"select data from DATA where ID = %s;",argv[0]);
    int rc;
    char * zErrMsg = 0;
    rc = sqlite3_exec(db,mommand,callback_song,0,&zErrMsg);
    if(rc != SQLITE_OK)
    {
        printf("数据发送失败!%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }else{
        printf("数据发送成功!\n");
    }

    //删除表中ID最小的数据
    char mommand1[1024];
    sprintf(mommand1,"delete from DATA where ID = %s;",argv[0]);
    char * zErrMsg0 = 0;
    rc = sqlite3_exec(db,mommand1,NULL,0,&zErrMsg0);
    if(rc != SQLITE_OK)
    {
        printf("数据删除失败,%s\n",zErrMsg0);
        sqlite3_free(zErrMsg0);
        return -1;
    }else{
        printf("数据删除成功!%s\n",argv[0]);
    }
    return 0;
}

//数据发送线程
void Take_sqlite(void)
{
    char *mommand = "select ID from DATA order by ID asc limit 1 offset 0;";
    int rc = 0;
    char * zErrMsg = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);  //加锁
        rc = sqlite3_exec(db,mommand,callback,0,&zErrMsg);
        if(rc != SQLITE_OK)
        {
            printf("数据发送失败,%s\n",zErrMsg);
            sqlite3_free(zErrMsg);
        }else{
            printf("数据发送成功\n");
        }
        pthread_mutex_unlock(&lock);
        sleep(TIME);
    }
}


int main(void)
{
    /*初始化日志*/
    FILE *log = NULL;
    log = InitLog(LOG);
    if(NULL == log)
    {
        printf("日志初始化失败!\n");
        return -1;
    }
    WriteLog("初始化成功!\n");
    printf("初始化成功!\n");

    FILE *retriever_fd;  //retriever的串口
    int PLC_fd;   //PLC的串口
    int T = 60; //定时60分钟

    if(init_Data_Delivery_Cloud(IP) < 0)  //初始化数据上传
    {
        WriteLog("init_Data_Delivery_Cloud:error!\n");
        printf("init_Data_Delivery_Cloud:erroe!!!\n");
        return -1;
    }
    WriteLog("数据上传初始化成功!\n");

    if(initWatchdog(T,SpectrumRetriever) < 0)  //初始化看门狗
    {
        WriteLog("initWatchdog:error!\n");
        printf("initWatchdog:error!!!\n");
        return -1;
    }
    WriteLog("初始化看门狗成功!\n");

    /*初始化458串口*/
    int fd = 0;  //485串口的文件描述符
    fd = InitSerialPort(MECTTY,9600,8,0,1);
    if(fd <= 0)
    {
        WriteLog("串口打开失败!\n");
        printf("串口打开失败!!!\n");
        return -1;
    }
    WriteLog("初始化看门狗成功!\n");

    MEC10 A;   //20公分的传感器
    A.temp = 0.0;
    A.vwc = 0.0;
    A.ec = 0.0;
    MEC10 B;   //40公分的传感器
    B.temp = 0.0;
    B.vwc = 0.0;
    B.ec = 0.0;
    MEC10 C;   //40公分的传感器
    C.temp = 0.0;
    C.vwc = 0.0;
    C.ec = 0.0;



    //打开数据库
    int rc;
    rc = sqlite3_open("/home/root/data.db",&db);
    if(rc != 0)
    {
        WriteLog("初始化数据库成功!\n");
        printf("数据库打开失败%s\n",sqlite3_errmsg(db));
        return -1;
    }else{
        WriteLog("数据库打开成功!\n");
        printf("数据库打开成功\n");
    }

    //初始化锁
    if(pthread_mutex_init(&lock,NULL) != 0)
    {
        WriteLog("初始化锁失败\n");
        printf("初始化锁失败\n");
        return -1;
    }else{
        WriteLog("初始化锁成功!\n");
        printf("初始化锁成功\n");
    }
    pthread_t thread;
    int ret_save = pthread_create(&thread,NULL,(void *)&Take_sqlite,NULL);
    if(ret_save != 0)
    {
        WriteLog("发送数据线程创建失败\n");
        printf("发送数据线程创建失败\n");
        return -1;
    }else{
        WriteLog("发送数据线程创建成功\n");
        printf("发送数据线程创建成功\n");
    }
    char mommand2[1024];
    char * zErrMsg2 = NULL;
    char data[300] = {0};

    time_t t;
    struct tm *tb;
    char times[20] = {0};
    
    while(1)
    {
        /*数据采集和数据整理*/
        /*20CM的传感器*/
        rc = MEC10_Collection(1,&A,fd);
        if(rc < 0)
        {
            WriteLog("传感器A数据处理错误!\n");
            printf("传感器A数据处理错误!\n");
            continue;
        }
        sleep(1);
        
        /*40CM的传感器*/
        rc = MEC10_Collection(2,&B,fd);
        if(rc < 0)
        {
            WriteLog("传感器B数据处理错误!\n");
            printf("传感器B数据处理错误!\n");
            continue;
        }
        sleep(1);

        /*60CM的传感器*/
        rc = MEC10_Collection(3,&C,fd);
        if(rc < 0)
        {
            WriteLog("传感器C数据处理错误!\n");
            printf("传感器C数据处理错误!\n");
            continue;
        }

        /*获取时间*/
        time(&t);
        tb = localtime(&t);
        sprintf(times,"%4d-%02d-%02d %02d:%02d",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday,tb->tm_hour,tb->tm_min);

        /*将3个传感器的数据整合到一起*/
        /*时间,R的SN,P的SN,A.temp,A.vwc,A.ec,B.temp,B.vwc,B.ec,C.temp,C.vwc,C.ec*/
        sprintf(data,"RS;%s;%s;100;%s;40;100;4;9;A;0;3;a;%.2f;A;1;37;a;%.2f;A;2;38;a;%.2f;B;0;3;a;%.2f;B;1;37;a;%.2f;B;2;38;a;%.2f;C;0;3;a;%.2f;C;1;37;a;%.2f;C;2;38;a;%.2f;1d4c\r\n",times,RETRIEVER,PUP,A.temp,A.vwc,A.ec,B.temp,B.vwc,B.ec,C.temp,C.vwc,C.ec);


        printf("整理好的数据: %s\n",data);
        
        sprintf(mommand2,"INSERT INTO DATA(data) VALUES('%s');",data);
        printf("%s\n",mommand2);

        pthread_mutex_lock(&lock);  //加锁
        rc = sqlite3_exec(db,mommand2,NULL,0,&zErrMsg2);
        if(rc != SQLITE_OK)
        {
            printf("插入数据失败,%s\n",zErrMsg2);
            sqlite3_free(zErrMsg2);

            pthread_mutex_unlock(&lock);
            WriteLog("插入失败!\n");    
            
        }else{
            printf("数据插入成功!\n");
        }
        pthread_mutex_unlock(&lock);  //解锁
        //喂狗
        loadWatchdog(SpectrumRetriever);
        //提取数据
        WriteLog(data);
    printf("数据发送完成!!!\n");
    sleep(300);
    }
}


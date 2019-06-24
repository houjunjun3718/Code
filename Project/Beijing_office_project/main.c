/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月21日 星期五 13时06分12秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sqlite3.h>
#include<pthread.h>


#include"DATA_Retriever.h"
#include"Spectrum_Data_Process.h"
#include"Shared.h"
#include"Log.h"
#include"IOT_Communication.h"
#include"EC20.h"

#define LOG "log_txt"
#define SQLITE "/home/root/data.db"

#define QUERID "00000001"    //企业ID

sqlite3 *sqlite_db;    //数据库操作符
pthread_mutex_t sqlite3_lock;     //数据库操作锁

char Retriever_Data[1024] = {0};


void *Data_Collection(void)
{
    /*初始化数据读取函数*/
    FILE *ReadData = InitRetriever();
    if(ReadData == NULL)
    {
        printf("串口初始化失败!\n");
        return NULL;
    }
    char data[1024] = {0};
    int rc = 0;
    char *zErrMsg = NULL;
    char mamand[1024] = {0};
    while(1)
    {
        rc = ReadRetriever(ReadData,data,1024);
        if(rc <= 0)
        {
            printf("数据读取失败!\n");
        }else{
            /*将读到的字符串存入数据库*/
            sprintf(mamand,"INSERT INTO DATA(data) VALUES('%s');",data);

            pthread_mutex_lock(&sqlite3_lock);
            rc = 0;
            rc = sqlite3_exec(sqlite_db,mamand,NULL,0,&zErrMsg);
            if(rc != SQLITE_OK)
            {
                printf("数据插入失败!%s\n",zErrMsg);
                sqlite3_free(zErrMsg);

                pthread_mutex_unlock(&sqlite3_lock);
            }
            printf("数据插入成功!");
            pthread_mutex_unlock(&sqlite3_lock);
        }

    }
}


int callback_send(void *NotUsed,int argc,char **argv,char **azColName)
{
    sprintf(Retriever_Data,"%s",argv[0]);
    return 0;
}


int callback(void *NotUsed,int argc,char **argv,char **azColName)
{
    int i;
    for(i = 0;i < argc;i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i] : "NULL");
    }
    long int ren = 0;
    char mommand[1024] = {0};
    sprintf(mommand,"select data from DATA where ID = %s;",argv[0]);
    int rc = 0;
    char *zErrMsg = NULL;
    rc = sqlite3_exec(sqlite_db,mommand,callback_send,0,&zErrMsg);
    if(rc != SQLITE_OK)
    {
        printf("数据读取失败!%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
    }
    printf("数据读取成功!\n");

    char mommand1[1024] = {0};
    sprintf(mommand1,"delete from DATA where ID = %s;",argv[0]);
    char *zErrMsg1 = NULL;
    rc = 0;
    rc = sqlite3_exec(sqlite_db,mommand1,NULL,0,&zErrMsg1);
    if(rc != SQLITE_OK)
    {
        printf("数据删除失败!%s\n",zErrMsg1);
        sqlite3_free(zErrMsg1);
        return -1;
    }
    printf("数据删除成功!\n");
    return 0;

}




int main(void)
{
    //初始化日志系统
    if(InitLog(LOG) == NULL)
    {
        printf("日志初始化失败!\n");
        return -1;
    }
    printf("日志初始化成功!");

    //初始化数据库
    int rc = 0;
    rc = sqlite3_open(SQLITE,&sqlite_db);
    if(rc != 0)
    {
        printf("数据库初始化失败!\n");
        return -1;
    }
    printf("数据库初始化成功!\n");

    rc = 0;
    rc = pthread_mutex_init(&sqlite3_lock,NULL);
    if(rc != 0)
    {
        printf("初始化锁失败!\n");
        return -1;
    }

    //启动数据采集线程
    pthread_t Spectrum_pthread;
    rc = 0;
    rc = pthread_create(&Spectrum_pthread,NULL,(void *)&Data_Collection,NULL);
    if(rc != 0)
    {
        printf("数据采集线程创建失败!\n");
        return -1;
    }
    printf("数据采集线程创建成功!\n");
    
    
    //启动数据发送
        //初始化数据发送
    void *pclient = NULL;
    pclient = Leadwin_IOT_Communicaton_Init();
    if(pclient == NULL)
    {
        printf("数据发送初始化失败!\n");
        return -1;
    }
    rc = 0;
    rc = Leadwin_IOT_Get(pclient,200);
    if(rc != 0)
    {
        printf("订阅失败\n");
        return -1;
    }

        //从数据库中读取数据
    char *mammand = "select ID from DATA order by ID asc limit 1 offset 0;";
    char *zErrMsg = NULL;
    PUP pup;
    time_t t;
    int i;
    char Send[1024] = {0};
    while(1)
    {
        sleep(10);
        pthread_mutex_lock(&sqlite3_lock);
        rc = 0;
        rc = sqlite3_exec(sqlite_db,mammand,callback,0,&zErrMsg);
        if(rc != SQLITE_OK)
        {
            printf("数据读取失败,%s\n",zErrMsg);
            sqlite3_free(zErrMsg);
        }
        printf("数据读取成功!\n");
        pthread_mutex_unlock(&sqlite3_lock);
        if('R' != Retriever_Data[0])
        {
            printf("数据库为空\n");
            continue;
        }

        //对数据进行处理
        rc = 0;
        rc = Spectrum_Data_Process(Retriever_Data,&pup);
        if(rc <= 0)
        {
            printf("数据处理失败!\n");
            continue;
        }
        time(&t);
        memset(Send,0,1024);
        sprintf(Send,"{\"querID\":\"%s\",\"time\":\"%ld\",\"Number_of_data\":\"%d\"",QUERID,t,rc);
        //对数据进行整合
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P0);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P1);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P2);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P3);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P4);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P5);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P6);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P7);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P8);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.P9);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PA);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PB);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PC);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PD);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PE);
        }else{
            goto sen;
        }
        if(rc > 0)
        {
            rc--;
            sprintf(Send,"%s,%s",Send,pup.PF);
        }else{
            goto sen;
        }
sen:
        sprintf(Send,"%s}",Send);
        rc = 0;

        rc = Leadwin_IOT_Update(pclient,Send);
        if(rc != 0)
        {
            printf("数据发送失败!\n");
            EC20_Restart();
            goto sen;
        }
        printf("%s\n",Send);
        memset(Retriever_Data,0,1024);
        //发送数据
            //发送数据就重启4G模块

    }
    return 0;
}






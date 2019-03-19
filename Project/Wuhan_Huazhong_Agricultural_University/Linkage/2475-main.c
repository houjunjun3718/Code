/*************************************************************************
	> File Name: 2475-main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年01月03日 星期四 17时03分30秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<sqlite3.h>
#include<pthread.h>

#include"watchdog_2475.h"
#include"Data_Delivery_Cloud.h"
#include"ReuseFunction.h"


#define IP "http://39.105.67.7/LWI-getVersion.php"
#define DATA_IP "http://39.105.67.7/LWI-pushData.php"


#define TIME 20

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
        printf("数据发送失败\n");
        return -1;
    }
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
    sprintf(mommand,"select data from DATA_2475 where ID = %s;",argv[0]);
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
    sprintf(mommand1,"delete from DATA_2475 where ID = %s;",argv[0]);
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
    char *mommand = "select ID from DATA_2475 order by ID asc limit 1 offset 0;";
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
    int fd;
    int ren;
    int i;
    spectrum data;
    char Sdata[200] = {0};
    char Ldata[20] = {0};
    i = 0;
    fd = initSpectrum("/dev/ttyO3",9600,8,0,1);   //初始化串口


    //打开数据库
    int rc;
    rc = sqlite3_open("/home/root/data.db",&db);
    if(rc != 0)
    {
        printf("数据库打开失败%s\n",sqlite3_errmsg(db));
        return -1;
    }else{
        printf("数据库打开成功\n");
    }

    //初始化锁
    if(pthread_mutex_init(&lock,NULL) != 0)
    {
        printf("初始化锁失败\n");
        return -1;
    }else{
        printf("初始化锁成功\n");
    }
    pthread_t thread;
    int ret_save = pthread_create(&thread,NULL,(void *)&Take_sqlite,NULL);
    if(ret_save != 0)
    {
        printf("发送数据线程创建失败\n");
        return -1;
    }else{
        printf("发送数据线程创建成功\n");
    }
    char mommand2[1024];
    char * zErrMsg2 = NULL;

    while(fd <= 0)
    {
        
        printf("文件名=%s,函数=%s,行号=%d 返回值 = %d error : initSpectrum fail!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);
        sleep(1);
        i++;
        if(i >= 10)
        {
            printf("文件名=%s,函数=%s,行号=%d,返回值 = %d error : initSpectrum Implementitaon timeout!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);  
        }
        fd = initSpectrum("/dev/ttyO2",9600,8,0,1);   
        
    }
    printf("文件名=%s,函数=%s,行号=%d,返回值 = %d correct : initSpectrum success!!!\n",__FILE__,__FUNCTION__,__LINE__,fd);  
    
    i = 0;
    while(init_Data_Delivery_Cloud(IP) < 0)
    {
        printf("文件名=%s,函数=%s,行号=%d error : init_Data_Delivery_Cloud fail!!!\n",__FILE__,__FUNCTION__,__LINE__);
        sleep(1);
        i++;
        if(i >= 10)
        {
            printf("文件名=%s,函数=%s,行号=%d error : init_Data_Delivery_Cloud Implementitaon timeout!!!\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
        }
    }
    printf("文件名=%s,函数=%s,行号=%d correct : init_Data_Delivery_Cloud success!!!\n",__FILE__,__FUNCTION__,__LINE__);

    time_t t;
    struct tm *tb;
    char times[20];

    while(1)
    {
        ren = read_Spectrum(fd,&data);
        if(ren < 0)
        {
            printf("文件名=%s,函数=%s,行号=%d,返回值=%d error : read_Spectrum read fail!!!\n",__FILE__,__FUNCTION__,__LINE__,ren);
            sleep(60);
            continue;
        }
        printf("文件名=%s,函数=%s,行号=%d,返回值=%d correct : read_Spectrum read success!!!\n",__FILE__,__FUNCTION__,__LINE__,ren);
        
        printf("时间 : %s\n",data.time);
        printf("电量 : %s\n",data.battery);
        printf("光照 : %s\n",data.PB);
        printf("湿度 : %s\n",data.PC);
        printf("温度 : %s\n",data.PD);
        
        memset(Ldata,0,20);
        TemperatureConversionFunction(data.PD,Ldata);
        printf("摄氏温度 : %s\n",Ldata);

        //获取本地时间
        time(&t);
        tb = localtime(&t);
        
        sprintf(times,"%4d-%02d-%02d %02d:%02d",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday,tb->tm_hour,tb->tm_min);
        printf("%s\n",times);

        sprintf(Sdata,"RS;%s;20180016;---;247513446;40;%s;4;3;A;0;3;a;%s;A;1;37;a;%s;A;2;38;a;%s;1d4c",times,data.battery,Ldata,data.PC,data.PB);
        printf("数据 : %s\n",Sdata);
//        comply_Data_Delivery_Cloud(DATA_IP,Sdata);
        
        
        sprintf(mommand2,"INSERT INTO DATA_2475(data) VALUES('%s');",Sdata);
        printf("%s\n",mommand2);
        pthread_mutex_lock(&lock);  //加锁
        rc = sqlite3_exec(db,mommand2,NULL,0,&zErrMsg2);
        if(rc != SQLITE_OK)
        {
            printf("插入数据失败,%s\n",zErrMsg2);
            
            sqlite3_free(zErrMsg2);

            pthread_mutex_unlock(&lock);
            
            return -1;
        }else{
            printf("数据插入成功!\n");
        }
        pthread_mutex_unlock(&lock);  //解锁


        printf("-------------------------\n");
        sleep(1800);
    }

    
    return 0;
}



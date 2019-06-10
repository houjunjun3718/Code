/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月19日 星期五 14时03分38秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sqlite3.h>
#include<pthread.h>


#include"Data_Delivery_Cloud.h"
#include"Retriever_collection.h"


#define IP "http://101.201.80.90/LWI-getVersion.php"
#define DATA_IP "http://101.201.80.90/LWI-pushData.php"
int fd;   //串口文件描述符
sqlite3 *db;  //数据库,
pthread_mutex_t lock;    //数据库操作锁

//将数据存入数据库的函数
void Save_sqlite(void)
{
    if(fd < 0)
    {
        printf("打开的文件错误\n");

    }
    char data[1024];
    char mommand[1024];
    int rc;
    char * zErrMsg = NULL;
    //从串口读取数据
    while(1)
    {
        printf("读串口\n");
        if(0 < Read_Retriever(fd,data,1024))
        {
            printf("%d\n",fd);
            //将数据存入数据库
            sprintf(mommand,"INSERT INTO DATA(data) VALUES('%s');",data);
            printf("mommand = %s\n",mommand);
            pthread_mutex_lock(&lock);
            rc = sqlite3_exec(db,mommand,NULL,0,&zErrMsg);
            if(rc != SQLITE_OK)
            {
                printf("插入数据失败,%s\n",zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                printf("数据插入成功,%s\n",data);
            }
            pthread_mutex_unlock(&lock);
        }
    }
}
//发送数据
int callback_song(void *NotUsed,int argc,char **argv,char **azColName)
{
    int i = 0;
    for(;i < argc;i++)
    {
        printf("%s = %s\n",azColName[i],argv[i] ? argv[i] : "NULL");
    }
    int a = 0;
    a = comply_Data_Delivery_Cloud(DATA_IP,argv[0]);
    
    if(a < 0)
    {
        /*
        sleep(60);
        a = comply_Data_Delivery_Cloud(DATA_IP,argv[0]);
         */
        printf("数据发送失败!!!\n");
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
    //数据发送,将数据发送到服务器,如果数据发送失败就重新发送.
    char mommand2[1024];
    sprintf(mommand2,"select data FROM DATA where ID = %s;",argv[0]);
    int rc;
    char * zErrMsg1 = 0;
    rc = sqlite3_exec(db,mommand2,callback_song,0,&zErrMsg1);
    if(rc != SQLITE_OK)
    {
        printf("数据发送失败!%s\n",zErrMsg1);
        sqlite3_free(zErrMsg1);
        return -1;
    }else
    {
        printf("数据发送成功!\n");
    }

    //删除表中ID最小的数据
    char mommand3[1024];

    sprintf(mommand3,"delete from DATA where ID = %s;",argv[0]);
    
    char * zErrMsg0 = 0;
    rc = sqlite3_exec(db,mommand3,NULL,0,&zErrMsg0);
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

void Take_sqlite(void)
{
    char *mommand1 = "select ID from DATA order by ID asc limit 1 offset 0;";
    int rc = 0;
    char * zErrMsg2 = NULL;
    while(1)
    {
        pthread_mutex_lock(&lock);  //加锁
        rc = sqlite3_exec(db,mommand1,callback,0,&zErrMsg2);
        if(rc != SQLITE_OK)
        {
            printf("数据发送失败,%s\n",zErrMsg2);
            sqlite3_free(zErrMsg2);
        }else{
            printf("数据发送成功\n");
        }
        pthread_mutex_unlock(&lock);  //解锁
        sleep(20);
    }
}



int main(void)
{
    init_Data_Delivery_Cloud(IP);
    fd = init_Retriever("/dev/ttyO1",9600,8,0,1);
    if(-1 == fd)
    {
        printf("USB error\n");
        return -1;
    }
    printf("USB OK\n");

    char * zErrMsg3 = NULL;
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


//    char data[1024];
    /*
    while(1)
    {
//        char data[1024];
        if(0 < Read_Retriever(fd,data,1024))
        {
            comply_Data_Delivery_Cloud(DATA_IP,data);
        }
    }
    */
    //创建两个线程,一个线程用于数据采集,一个线程用于数据发送.

    pthread_t thread1,thread2;

    int ret_save = pthread_create(&thread1,NULL,(void *)&Save_sqlite,NULL);
    if(ret_save != 0)
    {
        printf("采集线程创建失败\n");
    }else{
        printf("采集线程创建成功\n");
    }
    
    int ret_take = pthread_create(&thread2,NULL,(void *)&Take_sqlite,NULL);
    if(ret_take != 0)
    {
        printf("发送线程创建失败\n");
    }else{
        printf("发送线程创建成功\n");
    }
    
    while(1);


    exit_Data_Delivery_Cloud();
    Clock_Retriever(fd);
    return 0;
}




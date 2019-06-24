/*************************************************************************
	> File Name: Log.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月19日 星期五 09时56分50秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
#include"Log.h"

FILE *Log_Fd = NULL;
pthread_mutex_t Log_lock;

int WriteLog(const char *str);

/*初始化日志
 *参数:要打开的日志文件
 *返回值:成功返回文件流
 *       失败返回NULL;
 * */
FILE *InitLog(const char *log)
{
    //打开文件
    int add = 0;
    Log_Fd = fopen(log,"a+");   
    /*以附加方式打开可读写的文件.若文件不存在,则会建立该文件,如果文件存在,写入的数据会被加到文件尾后,即文件原先的内容会被保留.(原来的EOF符不保留)*/

    while(NULL == Log_Fd)
    {
        Log_Fd = fopen(log,"a+");
        add++;
        if(10 < add)
        {
            printf("打开失败\n");
            return NULL;
        }
    }

    if(pthread_mutex_init(&Log_lock,NULL) != 0)
    {
        printf("初始化日志锁失败!!!\n");
        return NULL;
    }

    WriteLog("初始化成功!!!");
    printf("初始化成功");
 
    return Log_Fd;
}

/*写日志
 * 参数: str   要写入的日志
 *
 * 返回值:  成功返回写入的字节数,
 *          失败返回-1;*/
int WriteLog(const char *str)
{
    pthread_mutex_lock(&Log_lock);
    static int add = 0;      //用于记录行数
    if(100000 < add)         //最多可以写10万行日志
    {
        fseek(Log_Fd,0,SEEK_SET);  //将偏移量偏移到文件头
        add = 0;
    }
    time_t t;
    struct tm * tb;
    char buff[1024] = {0};
    time(&t);
    tb = localtime(&t);
    sprintf(buff,"%4d-%02d-%02d %02d:%02d:%02d %s\r\n",tb->tm_year+1900,tb->tm_mon+1,tb->tm_mday,tb->tm_hour,tb->tm_min,tb->tm_sec,str);
    printf("%s\n",buff);
    fputs(buff,Log_Fd);
    fflush(Log_Fd);
    pthread_mutex_unlock(&Log_lock);
    return strlen(buff);
}




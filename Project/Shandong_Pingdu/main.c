/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月13日 星期四 22时18分20秒
    > 山东平度忠坚合作社项目
    > 项目介绍:种植面积200余亩,分别种有梨,桃,樱桃等水果,主要种植的是梨.
    > 传感器总共安装了13个点,每个点3个SMEC300传感器.每个SMEC300传感器都
    > 可以对土壤的温度,湿度,EC进行监测.温度是华氏度,湿度是体积含水量,EC
    > 是西门子.以农田中间的水泥道路为分界线,将南北风为两个监控区.
    > 程序介绍:本次采用阿里物联网进行通信,数据格式为JS格式.从Retriever 
    > 处接收到数据后直接存入数据库,数据在发送的时候再从数据库中读取数据
    > 对数据进行解析,整理成新的数据进行发送.数据发送是主线程,数据的接收
    > 是子线程.
 ************************************************************************/

#include<stdio.h>
#include<sqlite3.h>
#include<pthread.h>


#include"Log.h"

sqlite3 *sqlite_fd;    //数据库操作符
pthread_mutex_t sqlite_lock;   //数据库操作锁


int main(void)
{
    int rc;    
    /*初始化日志*/
    FILE *log = NULL;
    int Log = 0;
    
    log = InitLog("log.txt");
    if(NULL == log)
    {
        printf("错误:日志初始化失败!\n");
        return -1;
    }
    Log = WriteLog("日志初始化成功!");

    /*初始化数据库*/
    rc = 0;

    rc = sqlite3_open("/home/root/data.db",&sqlite_fd);   //打开数据库
    if(0 != rc)
    {
        WriteLog("错误:数据库打开化失败!");
        return -1;
    }
    WriteLog("数据库初始化成功!");

    rc = 0;
    rc = pthread_mutex_init(&sqlite_lock,NULL);
    if(0 != rc)
    {
        WriteLog("数据库锁初始化失败!")
    }

    /*初始化串口接收*/

    /*初始化发送*/

    /*启动数据接收线程*/

    /*启动数据发送*/
    //从数据库中读取数据,
    //对读到的数据进行拆分
    //将拆分后的数据整合成新的字符串
    //发送数据

    return 0;
}








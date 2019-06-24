/*************************************************************************
	> File Name: EC20.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月10日 星期一 09时57分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>


#include"EC20.h"

#include"Log.h"

/*迈冲工控机EC20自动拨号后台程序
* 姓名:候军军
* 电话:15847703113
* 邮箱:15847703113@163.com
* 时间:2019年6月10日
* 该部分函数主要针对迈冲工控机的B803,*/

/*关闭后台的自动拨号程序
* 参数1:拨号程序的字符串
* 返回值:成功返回0
*        失败返回一个小于0的数*/
int CloseDialer(char buff)
{
    char Log_buff[1024] = {0};

    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------进入关闭后台自动拨号程序的函数",__FILE__,__FUNCTION__,__LINE__);
    WriteLog(Log_buff);

    FILE *fd = popen("pidof quectel-CM","r");
    if(NULL == fd)
    {
        memset(Log_buff,0,1024);
        sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,-----获取拨号程序进程号失败",__FILE__,__FUNCTION__,__LINE__);
        WriteLog(Log_buff);
        return -1;
    }
    char ID[10] = {0};
    char PID[100] = {0};
    int rc = 0;

    fgets(ID,10,fd);
    
    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------拨号程序的进程号:%s",__FILE__,__FUNCTION__,__LINE__,ID);
    WriteLog(Log_buff);

    sprintf(PID,"kill -9 %s",ID);

    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------关闭拨号进程的命令:%s",__FILE__,__FUNCTION__,__LINE__,PID);
    WriteLog(Log_buff);

    pclose(fd);

    rc = system(PID);
    if(rc > 0)
    {
        memset(Log_buff,0,1024);
        sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------关闭拨号进程失败:%d",__FILE__,__FUNCTION__,__LINE__,rc);
        WriteLog(Log_buff);
        return -2;
    }
    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------关闭拨号进程成功:%d",__FILE__,__FUNCTION__,__LINE__,rc);
    WriteLog(Log_buff);
    
    return 0;
}

/*重启EC204G模块
* 参数:void
* 返回值:成功返回0,
*        失败返回负数*/
int start_EC20(void)
{
    char Log_buff[1024] = {0};

    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------进入重启4G模块函数",__FILE__,__FUNCTION__,__LINE__);
    WriteLog(Log_buff);

    int rc = 0;
    rc = system("echo -e \"AT+CFUN=1,1\r\n\">/dev/ttyUSB2");
    if(rc <= 0)
    {
        memset(Log_buff,0,1024);
        sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------重启4G模块失败:%d",__FILE__,__FUNCTION__,__LINE__,rc);
        WriteLog(Log_buff);
           
    }
    sleep(40);
    
    memset(Log_buff,0,1024);
    sprintf(Log_buff,"所属文件: %s,所属函数: %s,行号: %d,------重启4G模块成功:%d",__FILE__,__FUNCTION__,__LINE__,rc);
    WriteLog(Log_buff);

    return 0;
}

/*启动拨号上网程序*/
int start_yiyuan_4g(void)
{



    return 0;
}




/*EC20模块重启,
 * 无参数;
 * 返回值:返回0,成功
 *        返回值小于06,失败*/
int EC20_Restart(void)
{
    //关闭自动拨号程序
    FILE *fd = popen("pidof quectel-CM","r");
    char id[10] = {0};
    char PID[100] = {0};
    fgets(id,10,fd);
    printf("PID:%s\n",id);
    sprintf(PID,"kill -9 %s",id);    
    pclose(fd);
    printf("%s\n",PID);
    system(PID);
    printf("关闭自动拨号程 序!\n");

    //执行重启EC20模块的命令
    system("echo -e \"AT+CFUN=1,1\r\n\">/dev/ttyUSB2");
    sleep(40);

    printf("执行重启EC20模块!\n");
    //重新启动/etc/yiyuan_4g程序
    system("/etc/yiyuan_4g &");
    
    sleep(20);
    printf("启动yiyuan_4G模块");

    return 0;
}



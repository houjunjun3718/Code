/*************************************************************************
	> File Name: EC20.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月10日 星期一 09时57分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


#include"EC20.h"

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
    FILE *fd = popen("pidof quectel-CM","r");
    if(NULL == fd)
    {
        
    }
    char ID[10] = {0};
    char PID[100] = {0};


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



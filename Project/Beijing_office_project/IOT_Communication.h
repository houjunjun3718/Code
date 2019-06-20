/*************************************************************************
	> File Name: IOT_Communication.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年03月19日 星期二 13时17分07秒
    > 模块说明:本模块用于与阿里云平台之间的通信.
 ************************************************************************/

#ifndef _IOT_COMMUNICATION_H
#define _IOT_COMMUNICATION_H


/*设备三元组信息*/
#define PRODUCT_KEY "a1UhSR6QYIV"
#define DEVICE_NAME "60190002"
#define DEVICE_SECRET "t6EchRfzAxRHv4enRMCMPYhIyjAj3OkO"

#define TOPIC_UPDATE "/"PRODUCT_KEY"/"DEVICE_NAME"/update"  //数据发布话题
#define TOPIC_ERROR "/"PRODUCT_KEY"/"DEVICE_NAME"/update/error"  //错误发布话题
#define TOPIC_GET "/"PRODUCT_KEY"/"DEVICE_NAME"/get"  //命令订阅话题


//char command[1024] = {0};   //用于存放接收到的命令







/*
 *功能:初始化登录,创建MQTT客户端
 *参数:void
 *返回值:成功返回用于通信的句柄,
         失败返回NULL.
 *编写日期:2019-3-19 14:12
 * */
void *Leadwin_IOT_Communicaton_Init(void);



/*
 *功能:发布数据,设备向阿里云物联网平台发送数据
 *参数:pclient:MQTT句柄;
 *     update:发送的数据
 *     
 *返回值:成功返回0
         -1:数据发送失败
 *编写日期:2019-3-19 20:37
 * */
int Leadwin_IOT_Update(void *pclient,char *update);


/*
 *功能:发布错误,设备向阿里物联网平台发送错误信息
 *参数:pclient:MQTT句柄;
 *     error:发送的错误;
 *
 *返回值:成功返回0
            -1:数据发送失败;
 *编写日期:2019-3-27 09:45
 * */
int Leadwin_IOT_Error(void *pclient,char *error);


/*
 *功能:订阅命令,阿里物联网平台向设备发送命令(线程接收)
 *参数:pclient:MQTT句柄
 *     time_ms:超时时间
 *返回值:成功返回:0
         失败返回:-1
 *编写日期:2019-3-27 09:57
 * */
int Leadwin_IOT_Get(void *pclient,int time_ms);






#endif

/*************************************************************************
	> File Name: IOT_Communication.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年03月19日 星期二 13时17分07秒
    > 模块说明:本模块用于与阿里云平台之间的通信.
 ************************************************************************/

#ifndef _IOT_COMMUNICATION_H
#define _IOT_COMMUNICATION_H

/*定义一个结构体,设备三元组信息,登录超时时间,心跳时间,发送缓冲区的最大长度,接收缓冲区的最大长度,*/
typedef struct{
    char *Product_Key;  //产品KEY
    char *Device_Name;  //设备名字
    char *Device_Secret;//设备密码

    int Time_Out;       //登录超时时间
    int Heartbeat_Time; //心跳时间
    int read_buf_size;  //接收缓冲区的最大长度
    int write_buf_size; //发送缓冲区的最大长度
}Leadwin_Log_In_t,*Leadwin_Log_In_pt;

/*
 *功能:初始化登录,创建MQTT客户端
 *参数:mqtt_params:用于登录的结构体.
 *返回值:成功返回用于通信的句柄,
         失败返回NULL.
 *编写日期:2019-3-19 14:12
 * */
iotx_mqtt_param_t Leadwin_IOT_Communicaton_Init(Leadwin_Log_In_t mqtt_params);


#endif

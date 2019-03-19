/*************************************************************************
	> File Name: IOT_Communication.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年03月19日 星期二 13时16分51秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

#include"IOT_Communication.h"
#include"iot_import.h"
#include"iot_export.h"



iotx_mqtt_param_t Leadwin_IOT_Communicaton_Init(Leadwin_Log_In_t mqtt_params)
{
    IOT_SetLogLevel(IOT_LOG_DEBUG);  //开启阿里云物联网SDK的调试功能,该功能会影响到三元组生成通讯使用的密码.
    int domain_type = IOTX_CLOUD_REGION_SHANGHAI;
    
}

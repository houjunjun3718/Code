/*************************************************************************
	> File Name: IOT_Communication.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年03月19日 星期二 13时16分51秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"IOT_Communication.h"
#include"iot_import.h"
#include"iot_export.h"

static void _demo_message_arrive(void *pcontext,void *pclient,iotx_mqtt_event_msg_pt msg)  //订阅的回调函数
{
    iotx_mqtt_topic_info_pt ptopic_info = (iotx_mqtt_topic_info_pt)msg->msg;
    switch(msg->event_type)
    {
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            printf("--------------------------\n");
            printf("PacketId: %d\n",ptopic_info->packet_id);
            printf("Topic: '%.*s'(Length: %d)\n",
                  ptopic_info->topic_len,
                  ptopic_info->ptopic,
                  ptopic_info->topic_len);
            printf("--------------------------\n");
            /*将数据发送给串口*/
            //数据存放在ptopic_info->payload中,长度为ptopic_info->payload_len.

            printf("Payload: '%.*s' (Length: %d)\n",
                  ptopic_info->payload_len,
                  ptopic_info->payload,
                  ptopic_info->payload_len);
            printf("--------------------------\n");
            //判断串口是否打开,
            
            //调用串口发送函数将命令发送给Retriever



            break;
        default:
            printf("不应该到达这里\n");
            break;
    }
}


//MQTT客户端的回调函数(用于查看通信状态)
void event_handle(void *pcontext,void *pclient,iotx_mqtt_event_msg_pt msg) 
{
    uintptr_t packet_id = (uintptr_t)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;
    switch(msg->event_type)
    {
        case IOTX_MQTT_EVENT_UNDEF: 
            printf("undefined event occur.发生未定义的事件\n");
            break;
        case IOTX_MQTT_EVENT_DISCONNECT:
            printf("MQTT disconnect.MQTT断开连接\n");
            break;
        case IOTX_MQTT_EVENT_RECONNECT:
            printf("MQTT reconnect.MQTT重新连接\n");
            break;
        case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
            printf("subscribe success订阅成功,packet-id = %u\n",(unsigned int)packet_id); 
            break;
        case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
            printf("subscribe wait ack timeout,订阅等待ack超时,packet-id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
            printf("subscribe nack,订阅nack packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
            printf("unsubscribe success,取消订阅成功,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
            printf("unsubscribe timeout,取消订阅超时,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
            printf("unsubscribe nack,取消订阅nack,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
            printf("publish success,发布成功,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
            printf("publish timeout,发布超时,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_PUBLISH_NACK:
            printf("publish nack,发布nack,packet_id = %u\n",(unsigned int)packet_id);
            break;
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            printf("topic message arrived but without any related handle,主题消息到达但没有任何相关句柄: topic=%.*s,topic_msg=%.*s\n",topic_info->topic_len,topic_info->ptopic,topic_info->payload_len,topic_info->payload);
            break;
        case IOTX_MQTT_EVENT_BUFFER_OVERFLOW:
            printf("buffer overflow,缓冲区溢出,%s\n",(char *)msg->msg);
            break;
        default:
            printf("should NOT arrive here.不应该到达这里\n");
            break;
    }

}

void *receiver(void *pclient)
{
    while(1)
    {
        printf("**********************************\n");
        IOT_MQTT_Yield(pclient,200);
        sleep(10);
    }
}


void *Leadwin_IOT_Communicaton_Init(void)
{
    void *pclient;

    //开启阿里云物联网SDK的调试功能, 该功能会影响到三元组生成通讯使用的密码.
    IOT_SetLogLevel(IOT_LOG_DEBUG);  
    printf("开启阿里云物联网的SDK的日志\n");

    //设定登录服务器为上海.目前全球的物联网服务器有(上海,东京,新加坡,硅谷,弗吉尼亚,法兰克福)
    int domain_type = IOTX_CLOUD_REGION_SHANGHAI;
    IOT_Ioctl(IOTX_IOCTL_SET_DOMAIN,(void *)&domain_type);
    printf("设置登录服务器为上海\n");
    
    //设置设备登录方式,0=一机一密,1=一型一密.
    int dynamic_register = 0;
    IOT_Ioctl(IOTX_IOCTL_SET_DYNAMIC_REGISTER,(void *)&dynamic_register);
    printf("设置登录方式位一机一密\n");

    //设置三元组
    iotx_conn_info_pt pconn_info = NULL;
    if(0 != IOT_SetupConnInfo(PRODUCT_KEY,DEVICE_NAME,DEVICE_SECRET,(void **)&pconn_info))
    {
        printf("[ERROR]生成的账号密码错误!!!\n");
        return NULL;
    }
    printf("生成账号和密码!!!\n");

    //构建MQTT客户端
    iotx_mqtt_param_t mqtt_params;
    memset(&mqtt_params,0x0,sizeof(mqtt_params));
    mqtt_params.port = pconn_info->port;
    mqtt_params.host = pconn_info->host_name;
    mqtt_params.client_id = pconn_info->client_id;
    mqtt_params.username = pconn_info->username;
    mqtt_params.password = pconn_info->password;
    mqtt_params.pub_key = pconn_info->pub_key;

    mqtt_params.request_timeout_ms = 2000;
    mqtt_params.clean_session = 0;
    mqtt_params.keepalive_interval_ms = 60000;   //一分钟
    mqtt_params.read_buf_size = 1024;
    mqtt_params.write_buf_size = 1024;

    mqtt_params.handle_event.h_fp = event_handle;
    mqtt_params.handle_event.pcontext = NULL;
    
    pclient = IOT_MQTT_Construct(&mqtt_params);
    if(NULL == pclient)
    {
        printf("MQTT 客户端构建失败\n");
        return NULL;
    }

    return pclient;
}



int Leadwin_IOT_Update(void *pclient,char *update)
{
    //判断网络情况
    if(0 == IOT_MQTT_CheckStateNormal(pclient))
    {
        printf("数据网络中断\n");
        return -2;
    }
    //发送数据
    iotx_mqtt_topic_info_t topic_msg;  
    char msg_pub[1024] = {0};   //存放待发送的数据
    memset(&topic_msg,0x0,sizeof(iotx_mqtt_topic_info_t));
    strcpy(msg_pub,update);
    topic_msg.qos = IOTX_MQTT_QOS1;  //加密方式
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);
    if(0 > IOT_MQTT_Publish(pclient,TOPIC_UPDATE,&topic_msg))
    {
        printf("数据发送失败\n");
        return -1;
    }
    IOT_MQTT_Yield(pclient,200);
    return 0;
}



int Leadwin_IOT_Error(void *pclient,char *error)
{
    //判断网络情况
    if(0 == IOT_MQTT_CheckStateNormal(pclient))
    {
        printf("错误网络中断\n");
        return -2;
    }
    //发送数据
    iotx_mqtt_topic_info_t topic_msg;  
    char msg_pub[1024] = {0};   //存放待发送的数据
    memset(&topic_msg,0x0,sizeof(iotx_mqtt_topic_info_t));
    strcpy(msg_pub,error);
    topic_msg.qos = IOTX_MQTT_QOS1;  //加密方式
    topic_msg.retain = 0;
    topic_msg.dup = 0;
    topic_msg.payload = (void *)msg_pub;
    topic_msg.payload_len = strlen(msg_pub);
    if(0 > IOT_MQTT_Publish(pclient,TOPIC_ERROR,&topic_msg))
    {
        printf("错误发送失败\n");
        return -1;
    }
    IOT_MQTT_Yield(pclient,200);
    return 0;    
}



int Leadwin_IOT_Get(void *pclient,int time_ms)
{
    //判断网络情况
    if(0 == IOT_MQTT_CheckStateNormal(pclient))
    {
        printf("订阅网络中断\n");
        return -2;
    }
    if(0 > IOT_MQTT_Subscribe(pclient,TOPIC_GET,IOTX_MQTT_QOS1,_demo_message_arrive,NULL))
    {
        printf("IOT_MQTT_Subscribe() failed\n");
        return -1;
    }
    IOT_MQTT_Yield(pclient,200);
    //创建一个线程,定时去读一次数据
    pthread_t ntid;
    if(0 != pthread_create(&ntid,NULL,receiver,pclient))
    {
        printf("云平台订阅线程创建失败\n");
        return -1;
    }
    pthread_detach(ntid);
    return 0; 
}






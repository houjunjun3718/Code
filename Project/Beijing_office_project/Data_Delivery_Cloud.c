/*************************************************************************
	> File Name: nutriton_delivery_fruit.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月17日 星期三 16时05分12秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"curl/curl.h"
#include"Data_Delivery_Cloud.h"

/*************************************
 *
 * 
 *
 * **********************************/
size_t process_data_buffer(void *buffer,size_t size,size_t nmemb,char **user_p)
{
    char *pBuf = (char *)buffer;
    char *pReturn = malloc(size*nmemb+1);
    *user_p = pReturn;
    strcpy(pReturn,pBuf);
    
    size_t return_size = size*nmemb;

    return return_size;
}

/*************************************
 *初始化将传感器数据传输给云的功能
 *参数1:云的地址
 *返回值:成功返回0,
         失败返回-1.
 * **********************************/
int init_Data_Delivery_Cloud(char *cloud)
{
    //初始化libcurl
    CURLcode return_code;
    //初始化libcurl库,初始化所有已知设置.
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != return_code)
    {
        //调用错误处理函数

        return -1;
    }
    //初始化http,创建http传输的文件描述符
    CURL *easy_handle = curl_easy_init();
    if(NULL == easy_handle)
    {
        //调用错误处理函数,需要关闭libcurl库

        return-1;
    }
    //创建一个指针用于接收数据的指针
    char *pBuf = NULL;

    //配置http的传输信息
    curl_easy_setopt(easy_handle,CURLOPT_URL,cloud);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,&process_data_buffer);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,&pBuf);

    //发送http请求
    if(CURLE_OK != curl_easy_perform(easy_handle))
    {
        //调用错误处理函数

        return -1; 
    }
    //判断数据的传输是否正确
    printf("%s\n",cloud);

    //释放资源
    free(pBuf);
    pBuf = NULL;

    //关闭HTTP连接
    curl_easy_cleanup(easy_handle);

    return 0;
}


/*************************************
 *执行将传感器数据传输给云
 *参数1:云的地址
 *参数2:要传输的数据
 *返回值:成功返回0,
         失败返回-1.
 * **********************************/
int comply_Data_Delivery_Cloud(char *cloud,char *data)
{
    CURL *curl;
    char postthis[200];
    sprintf(postthis,"data=%s",data);
    curl = curl_easy_init();
    if(NULL == curl)
    {
        //错误处理函数
        
        return -1;
    }
    char *pBuf = NULL;
    
    curl_easy_setopt(curl,CURLOPT_URL,cloud);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,&process_data_buffer);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&pBuf);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postthis);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDSIZE,(long)strlen(postthis));

    if(CURLE_OK != curl_easy_perform(curl))
    {
        //错误处理函数 

        return -1;
    }
    //判断数据的传输是否正确
    printf("%s\n",pBuf);

    free(pBuf);
    pBuf = NULL;
    curl_easy_cleanup(curl);

    return 0;
}


/*************************************
 *清理函数,当不在调用该模块是使用
 *参数:无
 *返回值:无
 * **********************************/
 void exit_Data_Delivery_Cloud(void)
{
    curl_global_cleanup();
}





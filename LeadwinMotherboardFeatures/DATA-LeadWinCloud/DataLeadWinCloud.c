/*************************************************************************
	> File Name: DataLeadWinCloud.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月14日 星期日 16时31分12秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>

#include"DataLeadWinCloud.h"



size_t process_data_buffer(void *buffer,size_t size,size_t nmemb,char **user_p)
{
    char *pBuf = (char *)buffer;
    char *pReturn = malloc(size*nmemb+1);
    *user_p = pReturn;
    strcpy(pReturn,pBuf);

    size_t return_size = size*nmemb;

    return return_size;
}


/*初始化发送功能
 *参数cloud:云平台的地址.
 *返回值:成功返回0,
 *       失败返回-1.
 * */
int InitDataLeadWinCloud(char *cloud)
{
    //初始化libcurl
    CURLcode return_code;
    //初始化libcurl库,初始化所有以知设定
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != return_code)
    {
        printf("初始化curl_global_init失败!!!\n");
        return -1;
    }

    //初始化HTTP,创建HTTP传输的文件流
    CURL *easy_handle = curl_easy_init();
    if(NULL == easy_handle)
    {
        printf("初始化HTTP传输失败!!!\n");
        return -1;
    }

    //创建一个用于接收数据的指针
    char *pBuf = NULL;

    //配置HTTP的传输信息
    curl_easy_setopt(easy_handle,CURLOPT_URL,cloud);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,&process_data_buffer);
    curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,&pBuf);

    if(CURLE_OK != curl_easy_perform(easy_handle))
    {
        printf("HTTP请求失败!!!\n");
        return 1;
    }

    //释放资源
    free(pBuf);
    pBuf = NULL;

    //断开HTTP连接
    curl_easy_cleanup(easy_handle);

    return 0;
}



/*数据发送
 *参数cloud:云平台地址
 *参数data:上传的数据
 *返回值:成功返回0,
 *       失败返回-1.
 *       网络中断返回1.
 * */
int ComplyDataLeadWinCloud(char *cloud,char *data)
{
    CURL *curl;
    char postthis[200];
    sprintf(postthis,"data=%s",data);
    curl = curl_easy_init();
    if(NULL == curl)
    {
        printf("curl_easy_init 初始化失败!!!\n");
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
        printf("数据发送失败!!!\n");
        return 1;
    }

    free(pBuf);
    pBuf = NULL;
    curl_easy_cleanup(curl);

    return 0;
}



/*清理函数,当不在调用该模块时使用
 * 参数:无
 * 返回值:无
 * */
void ExitDataLeadWinCloud(void)
{
    curl_global_cleanup();
}


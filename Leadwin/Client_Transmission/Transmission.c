/*************************************************************************
	> File Name: Transmission.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月09日 星期二 10时47分35秒
 ************************************************************************/

#include<stdio.h>
#include"curl/curl.h"
#include<stdlib.h>
#include<string.h>



/*************************************************************************
 * 接收到数据时的回调子函数
 * 参数:buff
 *      size:of
 *      nmemb
 *      user_p
 *  返回值为接收到的字节数
 * **********************************************************************/
size_t process_data_buffer(void *buffer,size_t size,size_t nmemb,char** user_p)
{
    char* pBuf = (char*)buffer;
    char* pReturn = malloc(size*nmemb+1);
    *user_p = pReturn;
    strcpy(pReturn,pBuf);

    size_t return_size = size*nmemb;

    return return_size;
}


/*************************************************************************
 * 获取后台接口的版本号,版本号定义形式如V20180929.GET方法
 * 该接口可同时用来测试接口的连通情况
 * 无参数
 * 返回值:成功返回0
 *        失败返回-1
 * **********************************************************************/
int getVersion(void)
{
    //初始化libcurl
    CURLcode return_code;
    return_code = curl_global_init(CURL_GLOBAL_ALL);  //初始化libcurl库,初始化所有已知设置,除了后台传输服务.
    if(CURLE_OK != return_code)
    {
        return -1;
    }

    //获取easy handle
    CURL *easy_handle = curl_easy_init();//创建一个http传输的文件描述符
    if(NULL == easy_handle)
    {
        curl_global_cleanup();
        return -1;
    }
    char* pBuf = NULL;

    //设置easy handle属性
    curl_easy_setopt(easy_handle,CURLOPT_URL,"http://101.201.80.90/LWI-getVersion.php");//设置要发送数据的地址,
    curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION,&process_data_buffer);//设置回调函数,用于接收数据


    curl_easy_setopt(easy_handle,CURLOPT_WRITEDATA,&pBuf);//接收数据.存到pBuf中


    //执行数据请求
    curl_easy_perform(easy_handle);

    //释放资源
    free(pBuf);
    pBuf = NULL;

    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}

/*************************************************************************
 * 上送数据流,POST方法,
 * 参数格式:data=RS;2018-09-07 17:10;10180016;100;20180034;40;100;4;9;A;0;3;a;75.9;A;1;37;a;4.9;A;2;38;a;0.01;B;0;3;a;80.9;B;1;37;a;0.0;B;2;38;a;0.01;C;0;3;a;81.5;C;1;37;a;0.0;C;2;38;a;0.01;2633
 * 无参数 
 * 返回值:成功返回0
 *        失败返回-1
 * **********************************************************************/
int pushData(void)//simplepost.c
{
    CURL *curl;
    CURLcode res;
    time_t t = time(0);   
    char tmp[64];   
    strftime( tmp, sizeof(tmp), "%Y-%m-%d %X" , localtime(&t)  );   
    char postthis[200];
    sprintf(postthis, "data=RS;%s;10180016;100;20180034;40;100;4;9;A;0;3;a;75.9;A;1;37;a;4.9;A;2;38;a;0.01;B;0;3;a;80.9;B;1;37;a;0.0;B;2;38;a;0.01;C;0;3;a;81.5;C;1;37;a;0.0;C;2;38;a;0.01;2633", tmp);
    curl = curl_easy_init();
    if(curl) 
    {
        char* pBuf = 0; 
        curl_easy_setopt(curl, CURLOPT_URL, "http://101.201.80.90/LWI-pushData.php");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&process_data_buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,&pBuf);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
         /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
          * itself */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));
        res = curl_easy_perform(curl);
        /* always cleanup */
        
        //释放资源
//        delete[] pBuf;
        free(pBuf);
        pBuf = NULL;
        curl_easy_cleanup(curl);
                               
    }
        return 0;

}


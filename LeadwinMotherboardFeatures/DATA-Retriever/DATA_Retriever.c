/*************************************************************************
	> File Name: DATE_Retriever.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月09日 星期二 16时24分56秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>


#include"DATA_Retriever.h"
#include"Shared.h"

/*初始化Retriever串口
 * 无参数
 * 返回值为串口的文件描述符
 * */
FILE *InitRetriever(void)
{
    FILE *RetrieverFd = NULL;
    int fd = InitSerialPort(TTY,9600,8,0,1);
    if(-1 == fd)
    {
        printf("串口初始化失败!!!");
        return NULL;
    }
    printf("串口初始化成功!!!\n");

    RetrieverFd = fdopen(fd,"r+");
    if(NULL == RetrieverFd)
    {
        printf("串口文件打开失败!!!\n");
        perror("错误原因:");
        return NULL;
    }
    return RetrieverFd;   //返回串口的流文件符.
}


/* Retriever串口注销函数
 * 参数:文件描述符
 * 返回值:无
 * */
void Clock_Retriever(FILE *RetrieverFd)
{
    if(NULL == RetrieverFd)
    {
        printf("传入的参数错误!!!\n");
    }else{
        fclose(RetrieverFd);
        printf("串口关闭!!!\n");
    }
}


/* Retriever串口读取函数
 * RetrieverFd:串口的文件流
 * Data:数据缓冲区
 * DataLen:预计数据长度
 * return:成功返回读到的字节数,
 *        失败返回-1
 * */
int ReadRetriever(FILE *RetrieverFd,char *Data,int DataLen)
{
    int len;  //实际读到的字节数
    memset(Data,0,DataLen);
    if(fgets(Data,DataLen,RetrieverFd) == NULL)
    {
        printf("数据读取失败!!!\n");
        perror("失败原因:");
        return -1;
    }
    len = strlen(Data);
    return len;
}


/* Retriever串口写入函数
 * RetrieverFd:串口文件流
 * Data:数据缓冲区
 * DataLen:写的数据长度
 * return:成功返回写入的字节数
 *        失败返回-1
 * */
int WriteRetriever(FILE *RetrieverFd,char *Data,int Datalen)
{

}




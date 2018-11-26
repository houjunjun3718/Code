/*************************************************************************
	> File Name: Retriever.h
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月27日 星期一 15时28分00秒
 ************************************************************************/

#ifndef _RETRIEVER_H
#define _RETRIEVER_H

typedef struct
{
    char time[20];
    char Retriever_id[10];
    char Retriever_battery[10];
    char pup_id[10];
    char pup_signal[10];
    char pup_battery[10];
    char pup_A_temp[10];
    char pup_A_humidity[10];
    char pup_A_EC[10];
    char pup_B_temp[10];
    char pup_B_humidity[10];
    char pup_B_EC[10];
    char pup_C_temp[10];
    char pup_C_humidity[10];
    char pup_C_EC[10];
    char pup_D_temp[10];
    char pup_D_humidity[10];
    char pup_D_EC[10];
}Retriever_pup;

/******************************
 * Retriever串口的初始化
 * 参数０：要打开的串口
 * 参数１：波特率
 * 参数２：数据位
 * 参数３：校验位
 * 参数４：停止位
 * 返回值：成功返回串口的文件描述符
 *      　 失败返回－１
 * ***************************/
int initRetriever(char *tty,int baud,int datalen,int check,int stop);


/******************************
 * 关闭Retriever的串口
 * 参数０：串口的文件描述符
 * 无返回值
 * ***************************/
void closeRetriever(int fd);

/******************************
 * Retriever数据的处理函数
 * 参数０：串口的文件描述符
 * 参数１：回填的数据
 * 参数２：pup的个数
 * 返回值：成功返回读到的ｐｕｐ数据的个数
 * 　　　　失败返回－１；
 * ***************************/
int Retriever_data_processing_function(int fd,Retriever_pup *data,int pup,int UDP_fd);

#endif

/*************************************************************************
	> File Name: DATE_Retriever.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月09日 星期二 16时24分56秒
 ************************************************************************/

#ifndef _DATE_RETRIEVER_H
#define _DATE_RETRIEVER_H

#define TTY "/dev/ttyO1"


/*初始化Retriever串口函数
 * 无参数
 * 返回值为串口的文件流
 * */
FILE *InitRetriever(void);


/* Retriever串口注销函数
 * 参数:文件流
 * 返回值:无
 * */
void Clock_Retriever(FILE *RetrieverFd);

/* Retriever串口读取函数
 * RetrieverFd:串口的文件流
 * Data:数据缓冲区
 * DataLen:预计读的数据长度
 * return:成功返回读到的字节数,
 *        失败返回-1
 * */
int ReadRetriever(FILE *RetrieverFd,char *Data,int DataLen);

/* Retriever串口写入函数
 * RetrieverFd:串口文件流
 * Data:数据缓冲区
 * DataLen:写的数据长度
 * return:成功返回写入的字节数
 *        失败返回-1
 * */
int WriteRetriever(FILE *RetrieverFd,char *Data,int Datalen);

#endif

/*************************************************************************
	> File Name: Retriever_collection.h
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月19日 星期五 10时03分55秒
 ************************************************************************/

#ifndef _RETRIEVER_COLLECTION_H
#define _RETRIEVER_COLLECTION_H


/*********************************************
 *初始化Retriever的数据采集
 *参数 tty:串口的标示
       baud:波特率
       dataken:数据位
       check:校验位
       stop;停止位
 *返回值:成功返回串口的文件描述符
         失败返回-1
 * ******************************************/
FILE *init_Retriever(char *tty,int baud,int datalen,int check,int stop);

/*********************************************
 *注销Retriever的数据采集
 *参数:串口的文件描述符
 *返回值:无
 * ******************************************/
void Clock_Retriever(FILE *Retriever_fd);

/*********************************************
 *读取传感器的数据
 *参数:fd:串口文件描述符
       data:存储数据的缓冲区
       datalen:预计要读的数据
 *返回值:成功返回读到的字节数
         失败返回-1
 * ******************************************/
int Read_Retriever(FILE *Retriever_fd,char *data,int datalen);


/*********************************************
 * 向传感器写命令
 * 参数:Retriever_fd文件流
 *      data存储数据的缓冲区
 *      datalen数据的大小
 * 返回值:
 * ******************************************/
int write_Retriever(FILE *Retriever_fd,char *data,int datalen);

#endif

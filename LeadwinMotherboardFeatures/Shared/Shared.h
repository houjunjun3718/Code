/*************************************************************************
	> File Name: shared.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月08日 星期一 19时40分04秒
 ************************************************************************/

#ifndef _SHARED_H
#define _SHARED_H

/* 串口初始化
 * 参数:Tty:串口的标示
 *      Baud:串口波特率(0,50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200)
 *      DataBit:数据位(5,6,7,8)
 *      CheckDigit:校验位(0,1,2)
 *      StopBit:停止位(1,2)
 *返回值:成功返回串口的文件描述符
 *       失败返回-1
 * */
int InitSerialPort(char *Tty,int Baud,int DataBit,int CheckDigit,int StopBit);



#endif

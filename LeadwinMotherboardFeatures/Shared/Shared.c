/*************************************************************************
	> File Name: Shared.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月08日 星期一 19时40分04秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>

#include"Shared.h"



/* 串口初始化
 * 参数:Tty:串口的标示
 *      Baud:串口波特率
 *      DataBit:数据位
 *      CheckDigit:校验位
 *      StopBit:停止位
 *返回值:成功返回串口的文件描述符
 *       失败返回-1
 * */
int InitSerialPort(char *Tty,int Baud,int DataBit,int CheckDigit,int StopBit)
{
    int SerialPortFd;  //串口文件描述符
    int add = 0;   //用于计数
    struct termios options;  //串口配置结构体
    
    SerialPortFd = open(Tty,O_RDWR);  //以读写方式发开串口
    while(SerialPortFd <= 0)
    {
        sleep(1);
        add++;
        SerialPortFd = open(Tty,O_RDWR);
        if(add > 10);
        {
            add = 0;
            printf("串口打开失败,请查看串口参数时候正确:%s!!!\n",Tty);
            return -1;
        }
    }
    printf("串口打开成功:%d\n",SerialPortFd);
    add = 0;

    tcgetattr(SerialPortFd,&options);  //获取串口配置

    options.c_cflag |= (CLOCAL | CREAD); //忽略调制解调器线路状态和使能接收器
    options.c_cflag &= ~CSIZE;  //用数据位掩码清空数据位
    options.c_cflag &= ~CRTSCTS;  //关闭硬件流控

    switch(DataBit)  //设置数据位的个数
    {
        case 5:
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            printf("不应该到达这里,数据位设置错误:%d!!!\n",DataBit);
            close(SerialPortFd);   //关闭串口
            return -1;
    }
    printf("串口数据位设置成功:%d个数据位!!!\n",DataBit);
    
    switch(CheckDigit)  //设置数据校验位
    {
        case 0:
            options.c_cflag &= ~PARENB;   //不使用奇偶校验位
            options.c_iflag &= ~INPCK;    //不允许输出奇偶校验位
            break;
        case 2:
            options.c_cflag |= PARENB;    //使用奇偶校验位
            options.c_cflag |= PARODD;     //对输入使用奇偶校验,对输出使用偶校验
            options.c_iflag |= INPCK;     //允许输出奇偶校验位
            break;
        case 1:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;   //对输入使用奇偶校验,对输出使用奇校验
            options.c_iflag |= INPCK;     
            break;
        default:
            printf("不应该到达这里,校验位参数错误:%d!!!\n",CheckDigit);
            close(SerialPortFd);
            return -1;
    }
    printf("串口校验位设置成功,使用%d校验!!!\n",CheckDigit);

    switch(StopBit)   //设置停止位
    {
        case 1:
            options.c_cflag &= ~CSTOPB;   //设置为1个停止位
            break;
        case 2:
            options.c_cflag |= CSTOPB;    //设置位2个停止位
            break;
        default:
            printf("不应该到达这里,停止位参数错误:%d!!!\n",StopBit);
            close(SerialPortFd);
            return -1;
    }
    printf("串口停止位设置成功,使用%d个停止位!!!\n",StopBit);

    switch(Baud)   //设置串口波特率
    {
        case 0:
            cfsetospeed(&options,B0);
            break;
        case 50:
            cfsetospeed(&options,B50);
            break; 
        case 75:
            cfsetospeed(&options,B75);
            break;
        case 110:
            cfsetospeed(&options,B110);
            break;
        case 134:     //实际波特率为134.5
            cfsetospeed(&options,B134);
            break;
        case 150:
            cfsetospeed(&options,B150);
            break;
        case 200:
            cfsetospeed(&options,B200);
            break;
        case 300:
            cfsetospeed(&options,B300);
            break;
        case 600:
            cfsetospeed(&options,B600);
            break;
        case 1200:
            cfsetospeed(&options,B1200);
            break;
        case 1800:
            cfsetospeed(&options,B1800);
            break;
        case 2400:
            cfsetospeed(&options,B2400);
            break;
        case 4800:
            cfsetospeed(&options,B4800);
            break;
        case 9600:
            cfsetospeed(&options,B9600);
            break;
        case 19200:
            cfsetospeed(&options,B19200);
            break;
        case 38400:
            cfsetospeed(&options,B38400);
            break;
        case 57600:
            cfsetospeed(&options,B57600);
            break;
        /*
        case 76800:
            cfsetospeed(&options,B76800);
            break;
        */
        case 115200:
            cfsetospeed(&options,B115200);
            break;
        default:
            printf("不应该到达这里,波特路设置错误B:%d!!!n",Baud);
            close(SerialPortFd);
            return -1;
    }
    printf("串口波特率设置成功:%d!!!\n",Baud);

    options.c_iflag &= ~(IGNBRK | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);  //设置串口的输入配置

    options.c_oflag &= ~OPOST;  //设置串口的输出配置

    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);  //设置串口的本地配置
    tcflush(SerialPortFd,TCIOFLUSH);  //刷新读写缓存区

    tcsetattr(SerialPortFd,TCSANOW,&options);  //将配置加载到串口

    return SerialPortFd;
}



/*************************************************************************
	> File Name: PLC_communication.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月29日 星期四 17时57分36秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>

#include"PLC_communication.h"


/*************************************************************************
 *初始化通讯协议
 *功能:初始化串口,
 *参数:串口的参数
 *返回值:成功返回串口的文件描述符
 *       失败返回-1
 * **********************************************************************/
int PLC_communication_init(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;   //文件描述符
    struct termios options;   //串口信息的结构体
    fd = open(tty,O_RDWR);    //打开串口
    while(1 > fd)
    {
        //调用错误处理函数



        printf("PLC_communication_init:open serial port failed!!!\n");
        sleep(1);
    }

    tcgetattr(fd,&options);   //获取串口信息

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;   //屏蔽字符大小
    options.c_cflag &= ~CRTSCTS;  //关闭流控

    switch(datalen)  //设定数据位的长度
    {
        case 5:  //5个数据位
            options.c_cflag |= CS5;
            break;
        case 6:  //6个数据位
            options.c_cflag |= CS6;
            break;
        case 7:  //7个数据位
            options.c_cflag |= CS7;
            break;
        case 8:  //8个数据位
            options.c_cflag |= CS8;
            break;
        default:  //没有对应的数据位
            printf("PLC_communication_init:serial port datalen parameter error!!!\n");
            return -1;
    }

    switch(check)  //设置串口的停止位
    {
        case 0:   //无奇偶校验位
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~INPCK;
            break;
        case 1:   //打开奇校验
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_cflag |= INPCK;
            break;
        case 2:   //打开偶校验
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag |= INPCK;
            break;
        default:  //输入的参数错误
            //调用错误处理函数
            
            printf("PLC_communication_init:serial parity bit parameter error!!!\n");

            return -1; 
    }

    switch(stop)   //设置串口的停止位
    {
        case 1:  //一个停止位
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:  //两个停止位
            options.c_cflag |= CSTOPB;
            break;
        default:  //停止位参数设置错误
            
            //调用错误处理函数
            printf("PLC_communication_init:serial port stop bit parameter error!!!\n");
            return -1;
    }

    switch(baud)  //设置串口波特率
    {
        case 9600:  //设置波特率为9600;
            cfsetospeed(&options,B9600);
            break;
        case 115200:  //设置串口波特率为115200
            cfsetospeed(&options,B115200);
            break;
        default:  //波特率的参数错误
            
            printf("PLC_communication_init:serial port baud rate parameter error!!!\n");
            return -1;
    }
    
    options.c_iflag &= ~(IGNBRK | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcflush(fd,TCIFLUSH);  //清除输入缓存
    tcflush(fd,TCOFLUSH);  //清除输出缓存
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}


/******************************************************************************
 *基础-读函数(后期将该函数放到公共函数库)
 *功能:从文件描述符中读数据,基于read函数,使用时要注意.
 *参数0:文件描述符
 *参数1;读到数据存放的地址空间
 *参数2:预计要读的字节数
 *返回值:成功返回读到的字节个数
 *       失败返回-1
 * ***************************************************************************/
int read_function(int fd,char *data,int datalen)
{
    int ren;
    ren = read(fd,data,datalen);
    if(ren > 0)
    {
        printf("read data: %s\n",data);
        return ren;
    }
    return -1;
}

/******************************************************************************
 *基础-写函数(后期将该函数完善,放入公共函数库)
 *功能:向文件描述符写数据,基于write函数.
 *参数0:文件描述符
 *参数1:要写的数据
 *参数2:要写的字节数
 *返回值:成功返回写入的字节数
 *       失败返回-1;
 * ***************************************************************************/
int write_function(int fd,char *data,int datalen)
{
    int ren;
    ren = write(fd,data,datalen);
    if(ren = datalen)
    {
        printf("write data: %x\n",data);
        return ren;
    }
    return -1;
}


/******************************************************************************
 *文件监控函数--一般用于监控串口的读基础函数,后期放入公共函数库)
 *功能:监控文件描述符是否可以读
 *参数0:文件描述符
 *参数1:超时时间(单位: 秒)
 *返回值:成功返回可读的文件描述符
 *       超时或失败返回-1
 * ***************************************************************************/
int monitor_function(int fd,int time)
{
    int rc;
    struct timeval tv_timeout;
    fd_set set;
    FD_ZERO(&set);  //清空监控列表
    FD_SET(fd,&set);  //将文件描述符添加到监控列表
    tv_timeout.tv_sec = time;  //设定超时时间
    tv_timeout.tv_usec = 0;

    rc = select(fd + 1,&set,NULL,NULL,&tv_timeout);  //开始监控
    if(FD_ISSET(fd,&set)) 
    {
        return fd;
    }else{
        perror("select:");
        return -1;
    }
}

/*************************************************************************
 *PLC刷新函数
 *功能;给PLC指定的地址发送变化的数据,告诉PLC有数据发送过去啦
 *参数0:串口文件描述符
 *参数1:地址
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int PLC_refresh(int fd,int address)
{
    int Setaddr = address * 2 + 0x1000;   //PLC上的实际地址
    static int sign;   //向PLC发送的更新目标志,局部静态变量
    char refresh_data[200] = {0};  //发送刷新数据
    char answer_data[20];  //应答数据
    sign = sign+1;
    int r_l,r_h;
    if(sign > 100)
    {
        sign = 0;
    }
    refresh_data[0] = 0x02;
    refresh_data[1] = 0x31;
    sprintf(refresh_data+2,"%.4X",Setaddr & 0xffff);
    sprintf(refresh_data+6,"%.2X",2 & 0xff);
    sign = sign & 0xffff;
    r_l = sign & 0xff;
    r_h = sign / 0x100;
    sprintf(refresh_data + 8,"%.2X",r_l);
    sprintf(refresh_data + 10,"%.2X",r_h);

    refresh_data[12] = 0x03;
    int r_v = 0;
    int i;
    for(i = 1; i < 13;i++)
    {
        r_v += refresh_data[i];
    }
    r_v = r_v & 0xff;
    sprintf(refresh_data+13,"%.2X",r_v);
    int r_ren = 0;
//    printf("%s\n",refresh_data);
    r_ren = write_function(fd,refresh_data,15);
    if(r_ren < 0)
    {
        perror("PLC_refresh");
    }
    r_ren = monitor_function(fd,10);
    if(r_ren == fd)
    {
        r_ren = read_function(fd,answer_data,20);
        if(0x06 == answer_data[r_ren - 1])
        {
            return 0;
        }else{
            printf("answer_data[r_ren - 1] = %x\n",answer_data[r_ren -1]);
            printf("PLC_refresh error!!!\n");
            return -1;
        }
    }else{
        printf("PLC_refresh time out!!!\n");
        return -1;
    }

}


/*************************************************************************
 *向PLC写数据
 *功能:向PLC发送传感器数据
 *参数0:串口文件描述符
 *参数1:要发送的数据(要发送的数据全部是整型数据,PLC无法处理小数,要和PLC提前约定好)
 *参数2:要发送数据的个数
 *参数3:地址
 *参数4:超时时间
 *返回值:成功返回写入的字节个数
 *       失败返回-1
 * **********************************************************************/
int PLC_communication_write(int fd,char **buf,int buflen,int address,int time)
{
    int temp;    //取整后的数据
    int v = 0;   //校验和
    int Setaddr = address * 2 + 0x1000;   //PLC上的实际地址
    int l,h;
    int len;
    int i,ren;

    char write_data[200] = {0};   //发送出去的数据
    char read_data[20];    //接收到的应答数据

    len = 8 + buflen * 4 + 3;   //实际发送的字节数
    write_data[0] = 0x02;   //报文开始标志
    write_data[1] = 0x31;   //读/写标志位
    
    sprintf(write_data+2,"%.4X",Setaddr & 0xffff);  //将地址写入数组当中
    sprintf(write_data+6,"%.2X",buflen*2 & 0xff);  //将字节个数写入数组

    printf("33333333333\n");
    printf("%s",buf[0]);
    for(i = 0;i < buflen;i++)
    {
        temp = atof(buf[i+2]);    //将字符转成整型
        temp = temp & 0xffff;  //取出这个数据的低4位
        l = temp & 0xff;  //取出低2位字节
        h = temp / 0x100;  //取出高2位的字节
        sprintf(write_data + 8 + i*4,"%.2X",l);  //将低字节放到前面
        sprintf(write_data + 10 + i*4,"%.2X",h);  //将高字节放到后面
    }
    printf("99999999999\n");

    write_data[len -3] = 0x03;  //结束标志

    for(i = 1;i < len -2;i++)  //计算和校验
    {
        v += write_data[i];
    }
    v = v & 0xff;  //只去校验值的底8位
    sprintf(write_data + 8 + buflen*4 + 1,"%.2X",v);  //将校验码放入数组

    ren = write_function(fd,write_data,len);  //向PLC写数据
    if(ren < 0)
    {
        perror("PLC_communication_write:write_data");
        return -1;
    }

    ren = monitor_function(fd,time);  //监控串口是否有数据

    if(ren == fd)
    {
        ren = read_function(fd,read_data,20);  //读串口的数据
        if(0x06 == read_data[ren - 1])
        {
//            printf("read_data[ren -1] = %x\n",read_data[ren - 1]);
            //读到的数据正确,发送刷新
            if(0 == PLC_refresh(fd,400)) //-------------地址还不确定.
            {
                return buflen;
            }else{
                printf("PLC数据刷新失败!!!\n");
            }
        }else{
            printf("PLC数据发送失败!!!\n");
            return -1;
        }
    }else{
        printf("PLC数据写超时!!!\n");
        return -1;
    }
}


/*************************************************************************
 *从PLC读数据
 *功能:从PLC中读取施肥机的数据
 *参数0:串口文件描述符
 *参数1:要读的数据
 *参数2:要读的字节个数
 *参数3:要读的数据的地址
 *参数4:超时时间
 *返回值:成功返回读到的字节个数
 *       失败返回-1
 * **********************************************************************/
int PLC_communication_read(int fd,char *buf,int buflen,int address,int time);










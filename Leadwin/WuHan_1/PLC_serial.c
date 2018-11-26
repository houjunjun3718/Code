/*************************************************************************
	> File Name: PLC_serial.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月16日 星期四 14时08分36秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<stdlib.h>

#include"PLC_serial.h"


/*******************************************
 * PLC串口的初始化
 * 参数０：要打开的串口的地址
 * 参数１：波特率
 * 参数２：数据位
 * 参数３：校验位
 * 参数４：停止位
 * 返回值：成功返回文件描述符，失败返回-1；
 * ****************************************/
int initPLC_serial(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;  //文件描述符
    struct termios options;//串口信息的结构体
    fd = open(tty,O_RDWR);
    if(fd < 0)
    {
        printf("PLC_serial:open error!\n");
        return -1;
    }
    tcgetattr(fd,&options);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;//屏蔽字符大小
    options.c_cflag &= ~CRTSCTS;//流控
    switch (datalen)
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
        printf("PLC_serial:datalen error!\n");
        return -1;
    }
    switch (check)
    {
    case 0:        //无奇偶校验
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 1:        //奇校验
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
        options.c_iflag |= INPCK;
        break;
    case 2:        //偶校验
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    default:
        printf("PLC_serial:check error\n");
        return -1;
    }
    switch (stop) 
    {
    case 1:        //一个停止位
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:        //两个停止位
        options.c_cflag |= CSTOPB;
        break;
    default:
        printf("PLC_serial:stop error\n");
        return -1;
    }
    switch (baud)
    {
    case 9600:
        cfsetospeed(&options,B9600);
        break;
    case 115200:
        cfsetospeed(&options,B115200);
        break;
    default:
        printf("PLC_serial:baud error!\n");
        return -1;
    }
    options.c_iflag &= ~(IGNBRK|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    tcflush(fd,TCIFLUSH);//清楚输入缓存
    tcflush(fd,TCOFLUSH);//清除输出缓存
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}

/******************************************
 * 读函数
 * 参数０：文件描述符
 * 参数１：读到的数据
 * 参数２：预计要读的字节数
 * 返回值：成功返回读到的字节数，失败返回-1;
* ****************************************/
int read_function(int fd,char *data,int datalen)
{
    int ren;
    ren = read(fd,data,datalen);
    if(ren > 0)
    {
        return ren;
    }
    return -1;
}

/******************************************
 * 写函数
 * 参数０：文件描述符
 * 参数１：写的数据
 * 参数２：写的字节数
 * 返回值：成功返回写的字节数，失败返回-1;
* ****************************************/
int write_function(int fd,char *data,int datalen)
{
    int ren;
    ren = write(fd,data,datalen);
    if(ren == datalen)
    {
        return ren;
    }
    return -1;
}

/*****************************************
* 串口的监控
* 参数０：文件描述符(要监控的串口)
* 参数１：超时的时间(单位是秒)
* 返回值：成功返回串口文件描述符，失败返回-1;
* ****************************************/
int monitor_function(int fd,int time)
{
    int rc;
    struct timeval tv_timeout;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd,&set);
    tv_timeout.tv_sec = time;
    tv_timeout.tv_usec = 0;

    rc = select(fd+1,&set,NULL,NULL,&tv_timeout);
//    printf("rc = %d",rc);
    if(FD_ISSET(fd,&set))
    {
        return fd;
    }else
    {
        perror("select");
        return -1;    
    }
}

/*******************************************
 * PLC串口的读操作
 * 参数０：要读的串口
 * 参数１：读到的数据
 * 参数２：预计要读的数据的个数
 * 参数３：地址
 * 参数４：等待时间
 * 返回值：成功返回读到的字节数，失败返回-1;
 * *****************************************/
int read_PLC_serial(int fd,char **buf,int buflen,int address,int time)
{
    int Setaddr = address*2 + 0x1000 + 0x01;
    char read_command[20];
    char read_data[1024];
    int v = 0;
    int ren,i;
    int l,h;
    float temp;
    char jy[5];

    read_command[0] = 0x02;
    read_command[1] = 0x30;
    sprintf(read_command+2,"%.4X",Setaddr & 0xffff);  //将地址放在数组当中
//    printf("setaddr:%.4X\n",Setaddr & 0xffff);
    sprintf(read_command+6,"%.2X",buflen*2 & 0xff);   //将字节个数写入数组
//    printf("buflen:%.2X\n",buflen*2 & 0xff);
    read_command[8] = 0x03;
    for(i = 1;i <= 8;i++)          //计算和校验
    {
        v+=read_command[i];
    }
//    printf("v= %.4X\n",v);
    v = v & 0xff;                           //和校验只取低８位
    sprintf(read_command+9,"%.2X",v);               //将校验码放入数组
    ren = write_function(fd,read_command,10);
    /*
        for(i = 0;i <= 10;i++)
        {
            printf("%.2X ",read_command[i]);
        }
        printf("\n");
    */
    ren = monitor_function(fd,time);        //监控串口的应答  
//    printf("ren = %d\n",ren);
    if(ren == fd)
    {
        ren = read_function(fd,read_data,1024);
        for(i = 1;i < ren - 2;i++)   //校验
        {
            v += read_data[i];
        }
        v = v & 0xff;
        sprintf(jy,"%.2X",v);
        if((jy[0] == read_data[ren-2]) && (jy[1] == read_data[ren-1]))
        {
            for(i = 1;i < ren - 3;i++,i++)
            {
                l = read_data[i];
                h = read_data[i + 1];
                temp = h * 10 + l;
                sprintf(buf[i-1],"%f",temp);
            }
            return i;
        }else{
            printf("数据传输出错\n");
            return -1;
        }
    }else{
        printf("超时!\n");
        return -1;
    }
}


/******************************************
 * PLC串口的写操作
 * 参数０：要写的串口
 * 参数１：要写的数据
 * 参数２：要写的字节数
 * 参数３：地址
 * 参数４：等待时间
 * 返回值：成功返回写入的字的个数，失败返回-1;
 * ***************************************/
int write_PLC_serial(int fd,char **buf,int buflen,int address,int time)
{
    int temp;                               //取整后的数据
    int v = 0;                              //校验和
    int Setaddr = address * 2 + 0x1000;     //实际地址
    int l,h;                                //高低字节
    int len;
    int i,ren;

    char write_data[1024] = {0};                    //发送的数据
    char read_data[20];                     //接收的应答

    len = 8 + buflen*4 +3;                  //实际要发送的字节数
    write_data[0] = 0x02;                   //报文开始
    write_data[1] = 0x31;                   //写标志位
    sprintf(write_data+2,"%.4X",Setaddr & 0xffff);  //将地址放在数组当中
    sprintf(write_data+6,"%.2X",buflen*2 & 0xff);   //将字节个数写入数组
    for(i = 0;i < buflen;i++)
    {
        temp = atof(buf[i]);                //将字符串转换成浮点数据
        printf("%d\n",temp);                    //将浮点转换成整型，可能会丢失精度
        temp = temp & 0xffff;                   //将数据的低４位取出
        l = temp & 0xff;                        //取出低字节
        h = temp / 0x100;                       //取出高字节
        sprintf(write_data + 8 + i*4,"%.2X",l);         //将低字节放入数组
        sprintf(write_data + 10 + i*4,"%.2X",h);        //将高字节放入数组
    }
    write_data[len - 3] = 0x03;                         //结束标志
    for(i = 1;i < len - 2;i++)          //计算和校验
    {
        v+=write_data[i];
    }
    v = v & 0xff;                           //和校验只取低８位
    sprintf(write_data+8+buflen*4+1,"%.2X",v);               //将校验码放入数组
    
    printf("================================\n");
    ren = write_function(fd,write_data,len);      //写数据 
    if(ren < 0)
    {
        perror("write_function:");
    }
    
    ren = monitor_function(fd,time);        //监控串口的应答
//    printf("ren = %d\n",ren);
    if(ren == fd)
    {
        ren = read_function(fd,read_data,20); //读串口
        if(0x06 == read_data[ren-1])
        {
            //读到正确响应
            return buflen;
        }else
        {   //读到错误响应
            printf("%x\n",read_data[ren-1]);
            printf("写操作出错\n");
            return -1;
        }
    }else
    {   //写超时
        printf("写数据超时\n");
        return -1;
    }
}





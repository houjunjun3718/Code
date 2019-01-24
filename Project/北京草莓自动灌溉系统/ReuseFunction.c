/*************************************************************************
	> File Name: ReuseFunction.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月30日 星期五 09时54分50秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#include"ReuseFunction.h"



/*************************************************************************
 *温度转换函数
 *功能:将华氏温度转换为摄氏温度
 *参数0:华氏温度的字符串
 *参数1:摄氏温度的字符串
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int TemperatureConversionFunction(char *F,char *C)
{
    double ren;
    ren = ((atof(F)-32.0) * 5.0 / 9.0);  //浮点数据强转成整型数据舍弃小数点后面的精度.所以最后的温度只保留小数点后一位
    printf("TemperatureConversionFunction:ren = %.1f\n",ren);
    sprintf(C,"%.1f",ren);  //将数据转换成字符串存放到参数地址.
    return 0;
}



/*************************************************************************
 *数据放大函数
 *功能:PLC无法处理浮点数据,所以在使用中将浮点数据扩到到相应的整数
 *参数0:要扩大的字符串数据
 *参数1:扩大的倍数
 *参数2:扩大后的字符串
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int DataAmplificationFunction(char *data,int multiple,char *BigData)
{
    int ren;
    ren = atof(data) * multiple;
    printf("DataAmplificationFunction:ren = %d\n",ren);
    sprintf(BigData,"%d",ren);
}



/*************************************************************************
 * 串口的初始化
 * tty : 要打开的串口
 * baud : 串口的波特率
 * datalen : 串口的数据位长度
 * check : 串口的校验位
 * stop : 串口的停止位
 * 返回值:成功返回串口的文件描述符
 *        失败返回-1
 * **********************************************************************/
int open_serial(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;  //串口文件描述符
    struct termios options;   //串口信息的结构体
    fd = open(tty,O_RDWR);  //打开串口
    if(fd < 0)
    {
        printf("file : %s,function : %s,line number : %d,error : open serial port failed!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    tcgetattr(fd,&options);   //获取串口的当前信息

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;   //屏蔽字符大小
    options.c_cflag &= ~CRTSCTS; //关闭流控

    switch(datalen) //设定数据位长度
    {
        case 5:
        options.c_cflag |= CS5;  //5个数据位
        break;
        case 6:
        options.c_cflag |= CS6;  //6个数据位
        break;
        case 7:
        options.c_cflag |= CS7;  //7个数据位
        break;
        case 8:
        options.c_cflag |= CS8;  //8个数据位
        break;
        default:
        printf("file : %s,function : %s,line number : %d,error : datalen parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }

    switch(check)  //设置串口校验位
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
        default:
            printf("file : %s,function : %s,line number : %d,error : check digit parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
    }

    switch(stop)   //设置串口的停止位
    {
        case 1:    //一个停止位
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:    //两个停止位
            options.c_cflag |= CSTOPB;
            break;
        default:
            printf("file : %s,function : %s,line number : %d,error : stop bit parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
    }

    switch(baud)   //设置串口波特率
    {
        case 9600:   //设置串口波特率位9600
            cfsetospeed(&options,B9600);
            break;
        case 115200:  //设置串口波特率位115200
            cfsetospeed(&options,B115200);
            break;
        default:
            printf("file : %s,function : %s,line number : %d,error : baud rate parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
            return -1;
    }

    options.c_iflag &= ~(IGNBRK | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcflush(fd,TCIFLUSH);    //清除输入缓存
    tcflush(fd,TCOFLUSH);    //清除输出缓存
    tcsetattr(fd,TCSANOW,&options);  //将设置加载到串口的配置当中
    
    return fd;
}


/*************************************************************************
 * 串口关闭函数
 * fd : 串口的文件描述符
 * 返回值:无返回值
 * **********************************************************************/
void close_serial(int fd)
{
    if(fd >= 0)
    {
        close(fd);
        printf("Serial port is closed!!!\n");
        printf("file : %s,function : %s,line number : %d,normal : serial port is closed!!!\n",__FILE__,__FUNCTION__,__LINE__);
    }
}


/*************************************************************************
 * 读串口
 * fd : 串口的文件流
 * data : 存放读到的数据
 * datalen : 预计读到的数据的字节数
 * 返回值 : 成功返回实际读到的数据的字节数
 *          失败返回-1
 * **********************************************************************/
int read_serial(FILE *fd,char *data,int datalen)
{
    if(fd == NULL)
    {
        printf("file : %s,function : %s,line number : %d,error : fd parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }

    int len;  //读到的字节数
    
    memset(data,0,datalen);  //将数据缓存区清零
    if(fgets(data,datalen,fd) == NULL)
    {
        
        printf("file : %s,function : %s,line number : %d,error : fgets error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    len = strlen(data);  //计算读到的数据的长度

    return len;
}



/*************************************************************************
 * 写串口
 * fd : 串口的文件描述符
 * data : 要写的数据
 * datalen : 要写的字节数
 * 返回值: 成功返回0
 *         失败返回-1
 * **********************************************************************/
int write_serial(int fd,char *data,int datalen)
{
    if(fd <= 0)
    {
        printf("file : %s,function : %s,line number : %d,error : fd parameter error!!!\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    int len;
    len = write(fd,data,datalen);  //向串口写数据
    if(len != datalen)   //判断写的是否成功
    {
        printf("file : %s,function : %s,line number : %d,error : serial port failed to send!!!\n",__FILE__,__FUNCTION__,__LINE__);  
        return -1;
    }
    return 0;
}


/*************************************************************************
 * 串口可读的监控函数
 * fd : 串口的文件描述符
 * time : 等待时间
 * 返回值 : 文件可读返回文件描述符
 *          失败或超时返回-1
 * **********************************************************************/
int monitoring_serial(int fd,int time)
{
    int rc;
    struct timeval tv_timeout;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd,&set);
    tv_timeout.tv_sec = time;
    tv_timeout.tv_usec = 0;

    rc = select(fd + 1,&set,NULL,NULL,&tv_timeout);
    if(FD_ISSET(fd,&set))
    {
        return fd;
    }else{
        printf("file : %s,function : %s,line number : %d,error",__FILE__,__FUNCTION__,__LINE__);  
        perror(" : ");
        return -1;
    }
}




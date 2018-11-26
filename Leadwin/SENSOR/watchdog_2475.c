/*************************************************************************
	> File Name: watchdog_2475.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月17日 星期五 20时07分55秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/time.h>

#include"watchdog_2475.h"

/*********************************
 * 气象站的串口初始化函数
 * 参数０：要打开的串口的地址
 * 参数１：波特率
 * 参数２：数据位
 * 参数３：校验
 * 参数４：停止位
 * 返回值：成功返回串口的文件描述符，失败返回-1;
 * ******************************/
int initSpectrum(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;   //串口的文件描述符 
    struct termios options;
    fd = open(tty,O_RDWR | O_NOCTTY);
    if(fd < 0)
    {
        printf("WatchDog serial:open error!\n");
        return -1;
    }
    tcgetattr(fd,&options);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    switch(datalen)
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
            printf("WatchDog serial:datalen error!\n");
            return -1;
    }
    switch (check)
    {
        case 0:
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 1:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_iflag |= INPCK;
            break;
        case 2:
            options.c_cflag |= PARENB;
            options.c_cflag |= ~PARODD;
            options.c_iflag |= INPCK;
            break;
        default:
            printf("WatchDog serial:check erroe!\n");
            return -1;
    }
    switch (stop)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag &= CSTOPB;
            break;
        default:
            printf("WatchDog serial:stop error!\n");
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
            printf("WatchDog serial:baud error!\n");
            return -1;
    }
    options.c_iflag &= ~(IGNBRK|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    tcflush(fd,TCIFLUSH);
    tcflush(fd,TCOFLUSH);
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}


/*********************************
 * 气象站的串口关闭函数
 * 参数０：文件描述符
 * 返回值：成功返回０，失败返回－１
 * ******************************/
int cloceSpectrum(int fd)
{
    if(fd >= 0)
    {
        close(fd);
        return 0;
    }
    printf("串口已关闭或出现其他问题!!\n");
    return -1;
}

/*********************************
* 读串口（内部函数）
* 参数０：文件描述符
* 参数１：存数据的缓冲区
* 参数２：预计要读的字节数
* 返回值：成功返回实际读到的字节数，
*       　失败返回-1
* ********************************/
int read_watchdog_function(int fd,char *data,int datalen)
{
    int len;
    FILE *stand_fd;
    stand_fd = fdopen(fd,"r+");
    memset(data,0,datalen);
    if(fgets(data,datalen,stand_fd) == NULL)
    {
        printf("fgets error\n");
        return -1;
    }
    len = strlen(data);
//    fclose(stand_fd);
    /*
    len = read(fd,data,datalen);
    if(len <= 0)
    {
        printf("read_watchdog_function: error!!\n");
        return -1;
    }
    */
    return len;
}

/*********************************
* 写串口（用来写一些命令）
* 参数０：文件描述符
* 参数１：要写的数据的地址
* 参数２：要写的字节数
* 返回值：成功返回０；失败返回－１
* ********************************/
int write_watchdog_function(int fd,char *data,int datalen)
{
    int len;
    len = write(fd,data,datalen);
    printf("len = %d,datalen = %d\n",len,datalen);
    if(len != datalen)
    {
        perror("write:");
        printf("write_watchdog_function:error!!\n");
        return -1;
    }
    return 0;
}


/*********************************
* 串口可读的监控函数
* 参数０：文件描述符
* 参数１：等待时间
* 返回值：文件可读返回文件描述符，超时或失
* 败返回－１
* 若错误报告是成功表示读超时啦
* *********************************/
int monitoring_watchdog_function(int fd,int time)
{
    int rc;
    struct timeval tv_timeout;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd,&set);
    tv_timeout.tv_sec = time;
    tv_timeout.tv_usec = 0;

    rc = select(fd+1,&set,NULL,NULL,&tv_timeout);
    if(FD_ISSET(fd,&set))
    {
        return fd;
    }else{
        perror("select:");
        return -1;
    }
}

/*********************************
 * 发送通信开始命令的函数
 * 参数０：文件描述符
 * 返回值：成功返回０，失败返回－１
 * ******************************/
int communication_start_watchdog_function(int fd)
{
    int ren;
    char comm[40] = {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"};
    char data[10];
    ren = write_watchdog_function(fd,comm,32);
    if(ren < 0)
    {
        printf("write start data error!\n");
        return -1;
    }
    ren = monitoring_watchdog_function(fd,10);
    if(ren < 0)
    {
        printf("timeout!!\n");
        return -1;
    }
    ren = read_watchdog_function(fd,data,10);
    data[ren] = '\0';
    if(strcmp("OK\r\n",data) != 0)
    {
        printf("communication_start error\n");
        return -1;
    }
    return 0;
}

/*********************************
* 发送通信结束命令的函数
* 参数０：文件描述符
* 返回值：成功返回０，失败返回－１
* ********************************/
int communication_over_watchdog_function(int fd)
{
    int ren;
    char comm[40] = {"$X\r\n"};
    char data[10];
    ren = write_watchdog_function(fd,comm,4);
    if(ren < 0)
    {
        printf("write over data error!\n");
        return -1;
    }
    ren = monitoring_watchdog_function(fd,10);
    if(ren < 0)
    {
        printf("timeout!!\n");
        return -1;
    }
    ren = read_watchdog_function(fd,data,10);
    data[ren] = '\0';
    if(strcmp("OK\r\n",data) != 0)
    {
        printf("communication_start error\n");
        return -1;
    }
    return 0;
}

/*********************************
* 发送数据请求的函数
* 参数０：文件描述符
* 参数１：返回的数据的地址
* 返回值：成功返回字节个数，失败
* 返回－１
* *******************************/
int communication_data_watchdog_function(int fd,char *data)
{
    int ren;
    char comm[40] = {"$CC\r\n"};

    char value_buff[10];
    int val = 0;
    int checksum = 0;
    ren = write_watchdog_function(fd,comm,5);
    if(ren < 0)
    {
        printf("write data error!\n");
        return -1;
    }
    ren = monitoring_watchdog_function(fd,10);
    if(ren < 0)
    {
        printf("timeout!!\n");
        return -1;
    }
    ren = read_watchdog_function(fd,data,1024);
    data[ren] = '\0';
    printf("ren = %d\n",ren);
    if(ren >= 98)
    {
        strncpy(value_buff,data+92,4);
        value_buff[4] = '\0';
        sscanf(value_buff,"%x",&val);
        for(int i = 0;i < 92;i++)
        {
            checksum += (unsigned int)data[i];
        }
        if(val == checksum)
        {
            return ren;
        }else{
            printf("checksum error!\n");
            return -1;
        }
    }else{
        printf("data error\n");
        return -1;
    }
}

/*********************************
 * 读气象站中的数据
 * 参数０：串口的文件描述符
 * 参数１：读到的数据
 * 返回值：成功返回０，失败返回-1;
 * ＊＊＊＊＊＊注意＊＊＊＊＊＊＊
 * 气象站根据所插的传感器的不同，
 * 数据所取的位置也不相同，所以，
 * 在程序中要根据传感器的不同调整
 * 所取数据的位置．
 * ******************************/
int read_Spectrum(int fd,spectrum *data_sensor)
{
    
    //建立通信
    if(communication_start_watchdog_function(fd) != 0)
    {
        printf("communication_start_watchdog_function error!\n");
        return -1;
    }

    //接收数据
    char data[1024];
    int len = communication_data_watchdog_function(fd,data); 
    printf("%s\n",data);
    if(len < 0)
    {
        printf("communication_data_watchdog_function error!\n");
        return -1;
    }
    //断开通信
    if(communication_over_watchdog_function(fd) != 0)
    {
        printf("communication_over_watchdog_function error!\n");
        return -1;
    }
    //处理数据
    memcpy(data_sensor->time,data,16);
    data_sensor->time[16] = '\0';
    printf("%s\n",data_sensor->time);
    memcpy(data_sensor->PA,data+17,5);
    data_sensor->PA[5] = '\0';
    printf("%s\n",data_sensor->PA);
    memcpy(data_sensor->PB,data+23,5);
    data_sensor->PB[5] = '\0';
    memcpy(data_sensor->PC,data+29,5);
    data_sensor->PC[5] = '\0';
    memcpy(data_sensor->PD,data+35,5);
    data_sensor->PD[5] = '\0';
    memcpy(data_sensor->PE,data+41,5);
    data_sensor->PE[5] = '\0';
    memcpy(data_sensor->PF,data+47,5);
    data_sensor->PF[5] = '\0';
    memcpy(data_sensor->PG,data+53,5);
    data_sensor->PG[5] = '\0';
    memcpy(data_sensor->PH,data+59,5);
    data_sensor->PH[5] = '\0';
    memcpy(data_sensor->PI,data+65,5);
    data_sensor->PI[5] = '\0';
    memcpy(data_sensor->PJ,data+71,5);
    data_sensor->PJ[5] = '\0';
    memcpy(data_sensor->PK,data+77,5);
    data_sensor->PK[5] = '\0';
    memcpy(data_sensor->PL,data+83,5);
    data_sensor->PL[5] = '\0';
    memcpy(data_sensor->battery,data+89,3);
    data_sensor->battery[3] = '\0';
    return 0;    
}



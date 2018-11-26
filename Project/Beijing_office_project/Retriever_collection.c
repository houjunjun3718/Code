/*************************************************************************
	> File Name: Retriever_collection.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月19日 星期五 10时03分26秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#include"Retriever_collection.h"



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
int init_Retriever(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;
    struct termios options;
    fd = open(tty,O_RDWR);
    if(fd < 0)
    {
        //调用错误处理函数

        return-1;
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
        printf("Retriever:datalen error!\n");
        return -2;    
    }
    switch(check)                        
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
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
        default:
        printf("Retriever:check error!\n");
        return -3;
    }
    switch (stop)
    {
        case 1:
        options.c_cflag &= ~CSTOPB;
        break;
        case 2:
        options.c_cflag |= CSTOPB;
        break;
        default:
        printf("Retriever:stop error\n");
        return -4;     
    }
    switch(baud)
    {
        case 9600:
        cfsetospeed(&options,B9600);
        break;
        case 115200:
        cfsetospeed(&options,B115200);
        break;
        default:
        printf("Retriever:baudud error!\n");
        return -5;
    }
    options.c_iflag &= ~(IGNBRK | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcflush(fd,TCIFLUSH);
    tcflush(fd,TCOFLUSH);
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}

/*********************************************
 *注销Retriever的数据采集
 *参数:串口的文件描述符
 *返回值:无
 * ******************************************/
void Clock_Retriever(int fd)
{
    if(fd >= 0)
    {
        close(fd);
    }

}

/*********************************************
 *读取传感器的数据
 *参数:fd:串口文件描述符
       data:存储数据的缓冲区
       datalen:预计要读的数据
 *返回值:成功返回读到的字节数
         没有读到数据返回0
         失败返回-1
 * ******************************************/
int Read_Retriever(int fd,char *data,int datalen)
{
    FILE *Retriever_fd;
    Retriever_fd = fdopen(fd,"r+");
    if(Retriever_fd == NULL)
    {
        perror("fdopen:");
    }

    int len;
    memset(data,0,datalen);
    printf("memset ok\n");
    if(fgets(data,datalen,Retriever_fd) == NULL)
    {
        return 0;
    }
    printf("%s\n",data);
    printf("fgets ok\n");
    len = strlen(data);
//    fclose(Retriever_fd);
    return len;

}


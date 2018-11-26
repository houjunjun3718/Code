/*************************************************************************
	> File Name: main.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月19日 星期一 14时54分49秒
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

//打开串口
int initRetriever(void)
{
    int fd;
    struct termios options;
    fd = open("ttyO0",O_RDWR | O_NOCTTY);
    if(fd < 0)
    {

    }
    tcgetattr(fd,&options);         
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;                                     
    options.c_iflag &= ~INPCK;
    options.c_cflag &= ~CSTOPB;
    cfsetospeed(&options,B9600);
    options.c_iflag &= ~(IGNBRK|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);      
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    tcflush(fd,TCIFLUSH);
    tcflush(fd,TCOFLUSH);
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}

//关闭串口
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

//读串口
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
    return len;
}

//写串口
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

//串口监控
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



int main(void)
{
    int ren;
    char data[200];
    char a[10] = {"$RT/r/n"};
    int fd = initRetriever();
    printf("fd = %d\n",fd);
    ren = write_watchdog_function(fd,a,5);
    if(ren < 0)
    {
        printf("write_watchdog_function error!\n");
        return -1;
    }
    ren = monitoring_watchdog_function(fd,60);
    if(ren < 0)
    {
        printf("timeout!!\n");
        return -1;
    }

    ren = read_watchdog_function(fd,data,200);
    data[ren] = '\0';
    printf("%s\n",data);

    return 0;
}






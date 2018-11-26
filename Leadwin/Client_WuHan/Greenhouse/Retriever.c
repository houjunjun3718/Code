/*************************************************************************
	> File Name: Retriever.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年08月27日 星期一 15时27分38秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<error.h>
#include<sys/select.h>
#include<sys/time.h>




#include"Retriever.h"

#include"Client_UDP.h"


/******************************
 * Retriever串口的初始化
 * 参数０：要打开的串口
 * 参数１：波特率
 * 参数２：数据位
 * 参数３：校验位
 * 参数４：停止位
 * 返回值：成功返回串口的文件描述符
 *      　 失败返回－１
 * ***************************/
int initRetriever(char *tty,int baud,int datalen,int check,int stop)
{
    int fd;
    struct termios options;
    fd = open(tty,O_RDWR);
    if(fd < 0)
    {
        perror("open:");
        printf("Retriever_serial:open error!\n");
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
        printf("Retriever:datalen error!\n");
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
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
        default:
        printf("Retriever:check error!\n");
        return -1;
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
        return -1;
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
        return -1;
    }
    options.c_iflag &= ~(IGNBRK | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tcflush(fd,TCIFLUSH);
    tcflush(fd,TCOFLUSH);
    tcsetattr(fd,TCSANOW,&options);
    return fd;
}

/*********************************
 * 读串口（内部函数）
 * 参数０：文件流
 * 参数１：存数据的缓冲区
 * 参数２：预计要读的字节数
 * 返回值：成功返回实际读到的字节数，
 * 　　　　读完或失败返回０
 * ******************************/
int read_Retriever_function(FILE * fd,char *data,int datalen)
{
    int len;
    memset(data,0,datalen);
//    printf("read_Retriever_function 130\n");
    
    if(fgets(data,datalen,fd) == NULL)   //***************************************该函数是阻塞的
    {
//        printf("read_Retriever_function 139\n");
        return 0;
    }
    printf("%s\n",data);
    
    /********用fread来代替fgets*******/
//    fread(data,datalen,1,fd);


    len = strlen(data);
//    printf("read_Retriever_function len = %d\n",len);
    return len;
}
/*
int read_Retriever_function_ceshi(int fd,char *data,int datalen)
{
    int len;
    memset(data,0,datalen);
    len = read(fd,data,len);
    return len;
}
*/

/*********************************
 * 串口的监控函数
 * 参数０：文件描述符
 * 参数１：等待时间
 * 返回值：文件可读，返回文件描述符，
 * 　　　　超时或失败返回－１，
 * 若错误报告是成功表示等待超时
* *******************************/
int monitoring_Retriever_function(int fd,int time)
{
    int rc;
    struct set;
    fd_set set;
    struct timeval tv_timeout;
    FD_ZERO(&set);
    FD_SET(fd,&set);
    tv_timeout.tv_sec = time;
    tv_timeout.tv_usec = 0;

//    printf("monitoring_Retriever_function 164\n");
    rc = select(fd+1,&set,NULL,NULL,&tv_timeout);
//    printf("monitoring_Retriever_function 166\n");
    if(FD_ISSET(fd,&set))
    {
        return fd;
    }else{
        perror("select:");
        return -1;
    }
}

/*****************************
 * 拆分字符串函数
 * sep:分割的标志
 * str:待分割的字符串
 * size:回填的分割玩的字符串个数
* ****************************/
char **dataJRESS(char sep,const char *str,int *size)
{
    int count = 0,i;
    /*判断要分割字符串的个数*/
    for(i = 0;i < strlen(str);i++)
    {
        if(str[i] == sep)
        {
            count ++;
        }
    }
    /*开辟空间*/
    char **ret = calloc(++count,sizeof(char *));

    int lastindex = -1;
    int j = 0;
    /*拷贝字符串*/
    for(i = 0;i < strlen(str);i++)
    {
        if(str[i] == sep)
        {
            ret[j] = calloc(i - lastindex,sizeof(char));
           // printf("开辟空间\n");
            memcpy(ret[j],str + lastindex + 1,i - lastindex - 1);
           // printf("拷贝字符串\n");
            j++;
            lastindex = i;
        }
    }
//    printf("拷贝前面的字符串\n");
    //处理最后一个子串
    if(lastindex <= strlen(str) - 1)
    {
        ret[j] = calloc(strlen(str) - lastindex,sizeof(char));
        memcpy(ret[j],str + lastindex + 1,strlen(str) - 1 -lastindex);
        j++;
    }
//    printf("拷贝最后一个字符串\n");
    size = &j;
//    printf("回填是否成功\n");
    return ret;
}


/*******************************
 * 单个ｐｕｐ数据的处理函数
 * 参数０：等待处理的数据
 * 参数１：处理完成后的结构体
 * 参数２：
 * 返回值：成功返回０失败返回－１
 * ****************************/
int pup_processing_function(char *data,Retriever_pup *pup)
{
    char **rea;
    int *len;
    char and = 0;

    /*将数据以分号进行分割*/
//    printf("开始数据分割\n");
    rea = dataJRESS(';',data,len);
//    printf("数据分割成功\n");
//    printf("RS = %s\n",rea[0]);
    /*判断是否是RS数据*/
    if(0 == strcmp("RS",rea[0]))
    {
        strcpy(pup->time,rea[1]);
//        printf("%s\n",pup->time);
        strcpy(pup->Retriever_id,rea[2]);
        strcpy(pup->Retriever_battery,rea[3]);
        strcpy(pup->pup_id,rea[4]);
        strcpy(pup->pup_signal,rea[5]);
        strcpy(pup->pup_battery,rea[6]);

        and = rea[9][0];
        switch (and)
        {
            case 'A':
                strcpy(pup->pup_A_temp,rea[13]);
                strcpy(pup->pup_A_humidity,rea[18]);
                strcpy(pup->pup_A_EC,rea[23]);
            break;
            case 'B':
                strcpy(pup->pup_B_temp,rea[13]);
                strcpy(pup->pup_B_humidity,rea[18]);
                strcpy(pup->pup_B_EC,rea[23]);
            break;
            case 'C':
                strcpy(pup->pup_C_temp,rea[13]);
                strcpy(pup->pup_C_humidity,rea[18]);
                strcpy(pup->pup_C_EC,rea[23]);
            break;
            case 'D':
                strcpy(pup->pup_D_temp,rea[13]);
                strcpy(pup->pup_D_humidity,rea[18]);
                strcpy(pup->pup_D_EC,rea[23]);
            break;
            default:
            return 0;
        }
        and = rea[24][0];
        switch (and)
        {
            case 'B':
                strcpy(pup->pup_B_temp,rea[28]);
                strcpy(pup->pup_B_humidity,rea[33]);
                strcpy(pup->pup_B_EC,rea[38]);
            break;
            case 'C':
                strcpy(pup->pup_C_temp,rea[28]);
                strcpy(pup->pup_C_humidity,rea[33]);
                strcpy(pup->pup_C_EC,rea[38]);
            break;
            case 'D':
                strcpy(pup->pup_D_temp,rea[28]);
                strcpy(pup->pup_D_humidity,rea[33]);
                strcpy(pup->pup_D_EC,rea[38]);
            break;
            default:
            return 0;
        }
        and = rea[39][0];
        switch (and)
        {
            case 'C':
                strcpy(pup->pup_C_temp,rea[43]);
                strcpy(pup->pup_C_humidity,rea[48]);
                strcpy(pup->pup_C_EC,rea[53]);
            break;
            case 'D':
                strcpy(pup->pup_D_temp,rea[43]);
                strcpy(pup->pup_D_humidity,rea[48]);
                strcpy(pup->pup_D_EC,rea[53]);
            break;
            default:
            return 0;
        }
        and = rea[54][0];
        switch (and)
        {
            case 'D':
                strcpy(pup->pup_D_temp,rea[43]);
                strcpy(pup->pup_D_humidity,rea[48]);
                strcpy(pup->pup_D_EC,rea[53]);

            break;
            default:
            return 0;
        }
        return 0;
    }
    return -1;
}


/******************************
 * Retriever数据的处理函数
 * 参数０：串口的文件描述符
 * 参数１：回填的数据
 * 参数２：pup的个数
 * 返回值：成功返回读到的ｐｕｐ数据的个数
 * 　　　　失败返回－１；
 * ***************************/
int Retriever_data_processing_function(int fd,Retriever_pup *data,int quan,int UDP_fd)
{
    FILE *Retriever_fd;
//    printf("Retriever_data_processing_function 339\n");
    Retriever_fd = fdopen(fd,"r+"); //将文件描述符转换为标准流
//    printf("Retriever_data_processing_function 341\n");
    if(-1 == monitoring_Retriever_function(fd,400)) //串口的等待时间设定为４００秒
    {
        return -1;
    }
//    printf("串口有数据可读\n");
    char read_data[1024];
    int len;
    int val = 0;
    int i = 0;

    printf("--------------------\n");
    for(i = 0; i < quan;i++)
    {
        int checksum = 0;
        
        if(0 == read_Retriever_function(Retriever_fd,read_data,1024))
        {
            printf("data[%d]\n",i);
            return i;
        }
        
//        read_Retriever_function_ceshi(fd,read_data,1024);
//        printf("%s\n",read_data);

        /*判断数据是否是以ＲＳ开头的数据*/
        char value_buf[5];
        memcpy(value_buf,read_data,2);
        value_buf[2] = '\0';
        if(strcmp("RS",value_buf) != 0)
        {
//            i--;
            continue;
        }
        /*校验*/
        char value_buff[10];
        len = strlen(read_data);
        memcpy(value_buff,read_data + (len -6),4);
        value_buff[4] = '\0';
        sscanf(value_buff,"%x",&val);
        int j;
        for(j = 0;j < len - 6;j++)
        {
            checksum += (unsigned int)read_data[j];
        }
        if(val != checksum)
        {
            printf("%s\n",read_data);
            printf("val = %x,checksum = %x\n",val,checksum);
            printf("校验错误\n");
            return -1;
        }
        printf("校验成功\n");
        /*数据处理*/
        write_UDP(UDP_fd,read_data,len);
        if(0 != pup_processing_function(read_data,data+i))
        {
            printf("data[%d] error\n",i);
        }

    }
//    printf("Retriever_data_processing_function:error\n");
    return i;

}


/******************************
 * 关闭Retriever的串口
 * 参数０：串口的文件描述符
 * 无返回值
 * ***************************/
void closeRetriever(int fd)
{
    if(fd >= 0)
    {
        close(fd);
    }
    printf("串口以关闭或出现其他问题\n");

}


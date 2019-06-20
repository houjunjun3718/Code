/*************************************************************************
	> File Name: Data_Processing.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月19日 星期三 14时28分15秒
 ************************************************************************/
#define _XOPEN_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#include"Data_Processing.h"
#include"Log.h"

/*Retriever数据校验函数
 * 参数1:需要校验的数据
 * 返回值:成功返回0
 *        失败返回-1*/
int Retriever_tesr(char *data)
{   
    char value_buff[10];
    int len;
    len = strlen(data);
    if(len < 10)
    {
       //参数错误,调用错误处理函数
        WriteLog("校验函数传入的参数错误!"); 
       return -1;
    }
    unsigned int val = 0;
    unsigned int checksum = 0;
    memcpy(value_buff,data + (len - 6),4);
    value_buff[4] = '\0';
    sscanf(value_buff,"%x",&val);
    int j;
    for(j = 0;j < len -6;j++)
    {
       checksum += (unsigned int)data[j];
    }
    if(val != checksum)
    {
       //错误处理函数
        WriteLog("数据错误,校验函数传入的参数错误未通过!");
        return -1;
    }
    return 0;
}

/*Retriever数据类型判断*/
int Retriever_Data_Judge(char *data)
{   
    int len;
    len = strlen(data);
    if(10 > len)           
    {  
        //数据错误,调用错误处理函数
       
        return -1;         
    }  
    char Command[5];       
    memcpy(Command,data,2);
    Command[2] = '\0';     
    if(strcmp(Command,"RS") == 0)
    {  
        //这是一条RS命令   
        return 0;          
    }else if(strcmp(Command,"RD") == 0)
    {  
        //这是一条RD命令   
        return 1;          
    }else
    {  
        //数据错误,调用错误处理函数
        return -1;         
    }            
}
/*************************************************************                                                                                                                                                 
 * 字符串分割函
 * 参数1:分割的标志
 * 参数2:待分割的字符串
 * 参数3:分割完的字符串的个数
 * 返回值:成功返回字符串的二维数组
 *        失败返回NULL;
 * **********************************************************/
char **dataJRESS(char sep,const char *str,int *size)
{
    int count = 0;
    int i;
    for(i = 0;i < strlen(str);i++)
    {
        count++;
    }
    char **ret = calloc(++count,sizeof(char *));
 
    int lastindex = -1;
    int j = 0;
    for(i = 0;i < strlen(str);i++)
    {
        if(str[i] == sep)
        {
            ret[j] = calloc(i - lastindex,sizeof(char));
            memcpy(ret[j],str + lastindex + 1,i - lastindex - 1);
            j++;
            lastindex = i;
        }
    }
    if(lastindex <= strlen(str) - 1)
    {
        ret[j] = calloc(strlen(str) - lastindex,sizeof(char));
        memcpy(ret[j],str + lastindex + 1,strlen(str) - 1 - lastindex);
        j++;
    }
    size = &j;
    return ret;
}

/*传感器数据类型转换
 * 将频谱的数据类型转成我们自己的数据类型
 * 参数:频谱的数据类型
 * 返回值:我们自己的数据类型*/
int Data_Type_Conversion(char *type)
{
    //将type转成整形
    int ty = atoi(type);

    //判断type的数据类型
    switch(ty)
    {
        case 3:  //华氏温度
            return 100;
        case 5:  //空气湿度
            return 303;
        case 8:  //关照
            return 314;
        case 37: //土壤湿度
            return 103;
        case 38: //土壤EC
            return 104;
        
    }
    //
}

/*Retriever数据处理函数
* 参数1,原始的Retriever数据
* 参数2,用于返回的拆分开从新组合的小字符串
* 返回值:成功返回小字符串的个数
*        返回0表示校验失败
*        返回-1表示这不是一条数据
*        失败返回-2*/
int Retriever_Data_Processing(char *data,char **element)
{
    int add = 0;
    //对数据进行校验
    int rc = 0;
    rc = Retriever_tesr(data);
    if(rc < 0)
    {
        return 0;
    }

    //判断是否是一条数据
    rc = 0;
    rc = Retriever_Data_Judge(data);
    if(rc != 0)
    {
        WriteLog("这不是一条数据!");
        return -1;
    }
    
    //对数据进行拆分
    char **rea = NULL;
    int *len;
    WriteLog("开始对字符串进行分割!");

    rea = dataJRESS(';',data,len);
    if(rea == NULL)
    {
        WriteLog("数据分割失败");
        return -1;
    }

    //从新组织数据,将数据存入结构体
    //将时间转化成时间戳
    char time[20] = {0};
    sprintf(time,"%s:00",rea[1]);
    
    struct tm stm;
    strptime(time,"%Y-%m-%d %H:%M:%S",&stm);
    long ti = mktime(&stm);
    memset(time,0,20);
    sprintf(time,"时间戳: %ld",ti);
    WriteLog(time);

    PUP Pup_data;
    
    //Retriever电量
    sprintf(Pup_data.P_0,"\"%s;900;%s;%ld\"",rea[2],rea[3],ti);
    add++;
    
    //Pup电量
    sprintf(Pup_data.P_1,"\"%s;900;%s;%ld\"",rea[4],rea[6],ti);
    add++;

    //Pup信号
    sprintf(Pup_data.P_2,"\"%s;901;%s;%ld\"",rea[4],rea[5],ti);
    add++;

    //获取数据,构建字符串
    if(('A' == rea[9]) || 'B' == rea[9] || 'C' == rea[9] || 'D' == rea[9])
    {
        
    }

    return add;
}



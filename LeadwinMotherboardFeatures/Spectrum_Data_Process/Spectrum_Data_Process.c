/*************************************************************************
	> File Name: Spectrum_Data_Process.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月20日 星期四 22时16分53秒
 ************************************************************************/

#define _XOPEN_SOURCE
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#include"Log.h"
#include"Spectrum_Data_Process.h"

/*数据校验模块
 * 参数:原始数据
 * 返回值:成功返回0
 *        失败返回-1*/
int SpectrumData_Check(char *data)
{
    char value_buff[10];
    int len;
    len = strlen(data);
    if(len < 10)
    {
        //参数错误,调用错误处理函数

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

        return -1;
    }
    return 0;
}

/*************************************************************
 *判断命令函数
 *参数1:数据
 *返回值:0 RS数据char
         1 RD数据
         ...
 *数据错误返回-1;
 * **********************************************************/
int SpectrumData_Command(char *data)
{
    int len;
    len = strlen(data);\
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


/*数据拆分模块
 * 参数1:原始数据
 * 参数2:拆分的符号
 * 参数3:字符串的个数
 * 返回值:成功返回拆分后的字符串
 *        失败返回NULL*/
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


/*数据类型判断函数
 * 参数:频谱的数据类型
 * 返回值:我们自己定义的数据类型*/
int DataType_Conversion(char *type)
{
    //将字符串转成整型
    int Intype = 0;
    Intype = atoi(type);
    switch(Intype)
    {
        case 3:     //温度
            return 100;

        case 37:    //土壤湿度
            return 103;

        case 38:    //土壤EC
            return 104;

        case 8:     //光照
            return 314;

        case 5:     //空气湿度
            return 303;
        default:
            return -1;
    }
}


/*频谱设备数据处理函数
 * 参数1:原始数据
 * 参数2:返回的处理后的数据
 * 返回值:成功返回处理后的字符的个数
 *        失败返回-1*/
int Spectrum_Data_Process(char *data,PUP *element)
{
    //对数据进行校验
    int rc = 0;
    rc = SpectrumData_Check(data);
    if(rc != 0)
    {
        WriteLog("校验失败!");
        return -1;
    }

    rc = 0;
    rc = SpectrumData_Command(data);
    if(rc != 0)
    {
        WriteLog("这不是一条RS数据!");
        return -1;
    }

    //对数据进行拆分
    char **ret = NULL;
    int *size;
    ret = dataJRESS(';',data,size);
    if(ret == NULL)
    {
        WriteLog("数据分割失败!");
        return -1;
    }

    //对数据进行提取
    /*将时间格式转成时间戳*/
    struct tm stm;
    char times[20] = {0};
    sprintf(times,"%s:00",ret[1]);
    strptime(times,"%Y-%m-%d %H:%M:%S",&stm);
    long ti = mktime(&stm);

    int add = 0;
    /*Retriever电量*/
    sprintf(element->P0,"\"0\":\"%s;stat;900;%s;%ld\"",ret[2],ret[3],ti);
    add++;

    /*PUP的信号强度*/
    sprintf(element->P1,"\"1\":\"%s;stat;901;%s;%ld\"",ret[4],ret[5],ti);
    add++;

    /*PUP的电量*/
    sprintf(element->P2,"\"2\":\"%s;stat;900;%s;%ld\"",ret[4],ret[6],ti);
    add++;
    int type = 0;

    if('A' == ret[9][0] || 'B' == ret[9][0] || 'B' == ret[9][0] || 'D' == ret[9][0])
    {
        type = DataType_Conversion(ret[11]);
        sprintf(element->P3,"\"3\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[9],type,ret[13],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }
    
    type = 0;
    if('A' == ret[14][0] || 'B' == ret[14][0] || 'B' == ret[14][0] || 'D' == ret[14][0])
    {
        type = DataType_Conversion(ret[16]);
        sprintf(element->P4,"\"4\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[14],type,ret[18],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    type = 0;
    if('A' == ret[19][0] || 'B' == ret[19][0] || 'B' == ret[19][0] || 'D' == ret[19][0])
    {
        type = DataType_Conversion(ret[21]);
        sprintf(element->P5,"\"5\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[19],type,ret[23],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }
    
    type = 0;
    if('A' == ret[24][0] || 'B' == ret[24][0] || 'B' == ret[24][0] || 'D' == ret[24][0])
    {
        type = DataType_Conversion(ret[26]);
        sprintf(element->P6,"\"6\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[24],type,ret[28],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    type = 0;
    if('A' == ret[29][0] || 'B' == ret[29][0] || 'B' == ret[29][0] || 'D' == ret[29][0])
    {
        type = DataType_Conversion(ret[31]);
        sprintf(element->P7,"\"7\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[29],type,ret[33],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    type = 0;
    if('A' == ret[34][0] || 'B' == ret[34][0] || 'B' == ret[34][0] || 'D' == ret[34][0])
    {
        type = DataType_Conversion(ret[36]);
        sprintf(element->P8,"\"8\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[34],type,ret[38],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    type = 0;
    if('A' == ret[39][0] || 'B' == ret[39][0] || 'B' == ret[39][0] || 'D' == ret[9][0])
    {
        type = DataType_Conversion(ret[41]);
        sprintf(element->P9,"\"9\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[39],type,ret[43],ti);
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[44][0] || 'B' == ret[44][0] || 'B' == ret[44][0] || 'D' == ret[44][0])
    {
        type = DataType_Conversion(ret[46]);
        sprintf(element->PA,"\"A\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[44],type,ret[48],ti); 
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[49][0] || 'B' == ret[49][0] || 'B' == ret[49][0] || 'D' == ret[49][0])
    {
        type = DataType_Conversion(ret[51]);
        sprintf(element->PB,"\"B\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[49],type,ret[53],ti);   
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[54][0] || 'B' == ret[54][0] || 'B' == ret[54][0] || 'D' == ret[54][0])
    {
        type = DataType_Conversion(ret[56]);
        sprintf(element->PC,"\"C\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[54],type,ret[58],ti);  
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[59][0] || 'B' == ret[59][0] || 'B' == ret[59][0] || 'D' == ret[59][0])
    {
        type = DataType_Conversion(ret[61]);
        sprintf(element->PD,"\"D\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[59],type,ret[63],ti);       
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[64][0] || 'B' == ret[64][0] || 'B' == ret[64][0] || 'D' == ret[64][0])
    {
        type = DataType_Conversion(ret[66]);
        sprintf(element->PE,"\"E\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[64],type,ret[68],ti);   
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }

    if('A' == ret[69][0] || 'B' == ret[69][0] || 'B' == ret[69][0] || 'D' == ret[69][0])
    {
        type = DataType_Conversion(ret[71]);
        sprintf(element->PF,"\"F\":\"%s%s;data;%d;%s;%ld\"",ret[4],ret[69],type,ret[73],ti);        
        add++;
    }else{
        WriteLog("数据处理完成!");
        return add;
    }
    printf("数据运行错误!\n");

}




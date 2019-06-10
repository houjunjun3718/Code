/*************************************************************************
	> File Name: Reteiever_Processing.c
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月29日 星期一 10时10分13秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#include"Reteiever_Processing.h"


/*************************************************************
 * 数据校验函数
 * 参数1:待校验数据(字符串)
 * 返回值:正确返回0
 *        错误返回-1;
 * **********************************************************/
int check_data(char *data)
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
 * 字符串分割函数
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

/*************************************************************
 *判断命令函数
 *参数1:数据
 *返回值:0 RS数据char
         1 RD数据
         ...
 *数据错误返回-1;
 * **********************************************************/
int dataCommand(char *data)
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

/*************************************************************
 *retriever数据处理函数
 *参数1:待处理的数据,(字符串)
 *参数2:处理好的数据,(结构体)
 *参数3:
 *返回值:成功返回0,
 *       失败返回-1,
 * **********************************************************/
int Reteiever_Processing(char *data,PUP *pup_data)
{
    //判断数据是否是RS数据
    printf("判断数据类型!!!\n");
    if(0 != dataCommand(data))
    {
        //对应的处理函数
    }
    //对数据进行校验
    printf("数据校验!!!\n");
    if(0 != check_data(data))
    {
        //对应的错误处理函数
    }
    //对数据进行分割
    char **rea = NULL;
    int *len;
    printf("数据分割!!!\n");
    rea = dataJRESS(';',data,len);
    if(rea == NULL)
    {
        //分割失败,调用错误处理函数
    }
    //将数据放入结构体
    
    printf("数据存放!!!\n");
    //放入时间
    strcpy(pup_data->time,rea[1]);
    //放入retriever的id
    strcpy(pup_data->Retriever_id,rea[2]);
    //放入retriever的电量
    strcpy(pup_data->Retriever_battery,rea[3]);
    //放入pup的id
    strcpy(pup_data->pup_id,rea[4]);
    //放入pup的信号强度
    strcpy(pup_data->pup_signal,rea[5]);
    //放入pup的电量
    strcpy(pup_data->pup_battery,rea[6]);
    //判断数据的端口第一个端口
    char and = rea[9][0];
    switch(and)
    {
        case 'A':
            strcpy(pup_data->pup_A0,rea[13]);
            strcpy(pup_data->pup_A1,rea[18]);
            strcpy(pup_data->pup_A2,rea[23]);
        break;
        case 'B':
            strcpy(pup_data->pup_B0,rea[13]);
            strcpy(pup_data->pup_B1,rea[18]);
            strcpy(pup_data->pup_B2,rea[23]);
        break;
        case 'C':
            strcpy(pup_data->pup_C0,rea[13]);
            strcpy(pup_data->pup_C1,rea[18]);
            strcpy(pup_data->pup_C2,rea[23]);
        break;
        case 'D':
            strcpy(pup_data->pup_D0,rea[13]);
            strcpy(pup_data->pup_D1,rea[18]);
            strcpy(pup_data->pup_D2,rea[23]);
        break;
        default:
        return 0;            
    }
    
    //判断数据的端口第二个端口
    and = rea[24][0];
    switch(and)
    {
        case 'A':
            strcpy(pup_data->pup_A0,rea[28]);
            strcpy(pup_data->pup_A1,rea[33]);
            strcpy(pup_data->pup_A2,rea[38]);
        break;
        case 'B':
            strcpy(pup_data->pup_B0,rea[28]);
            strcpy(pup_data->pup_B1,rea[33]);
            strcpy(pup_data->pup_B2,rea[38]);
        break;
        case 'C':
            strcpy(pup_data->pup_C0,rea[28]);
            strcpy(pup_data->pup_C1,rea[33]);
            strcpy(pup_data->pup_C2,rea[38]);
        break;
        case 'D':
            strcpy(pup_data->pup_D0,rea[28]);
            strcpy(pup_data->pup_D1,rea[33]);
            strcpy(pup_data->pup_D2,rea[38]);
        break;
        default:
        return 0;            
    }
        
    //判断数据的端口第三个端口
    and = rea[39][0];
    switch(and)
    {
        case 'A':
            strcpy(pup_data->pup_A0,rea[43]);
            strcpy(pup_data->pup_A1,rea[48]);
            strcpy(pup_data->pup_A2,rea[53]);
        break;
        case 'B':
            strcpy(pup_data->pup_B0,rea[43]);
            strcpy(pup_data->pup_B1,rea[48]);
            strcpy(pup_data->pup_B2,rea[53]);
        break;
        case 'C':
            strcpy(pup_data->pup_C0,rea[43]);
            strcpy(pup_data->pup_C1,rea[48]);
            strcpy(pup_data->pup_C2,rea[53]);
        break;
        case 'D':
            strcpy(pup_data->pup_D0,rea[43]);
            strcpy(pup_data->pup_D1,rea[48]);
            strcpy(pup_data->pup_D2,rea[53]);
        break;
        default:
        return 0;            
    }

    //判断数据的端口第四个端口
    and = rea[54][0];
    switch(and)
    {
        case 'A':
            strcpy(pup_data->pup_A0,rea[58]);
            strcpy(pup_data->pup_A1,rea[63]);
            strcpy(pup_data->pup_A2,rea[68]);
        break;
        case 'B':
            strcpy(pup_data->pup_B0,rea[58]);
            strcpy(pup_data->pup_B1,rea[63]);
            strcpy(pup_data->pup_B2,rea[68]);
        break;
        case 'C':
            strcpy(pup_data->pup_C0,rea[58]);
            strcpy(pup_data->pup_C1,rea[63]);
            strcpy(pup_data->pup_C2,rea[68]);
        break;
        case 'D':
            strcpy(pup_data->pup_D0,rea[58]);
            strcpy(pup_data->pup_D1,rea[63]);
            strcpy(pup_data->pup_D2,rea[68]);
        break;
        default:
        return 0;            
    }

    return 0;
}

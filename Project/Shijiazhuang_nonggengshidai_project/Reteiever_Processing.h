/*************************************************************************
	> File Name: Reteiever_Processing.h
	> Author: HJJ
	> Mail: 15847703113@163.com
	> Created Time: 2018年10月29日 星期一 10时10分14秒
 ************************************************************************/

#ifndef _RETEIEVER_PROCESSING_H
#define _RETEIEVER_PROCESSING_H


typedef struct
{
    char time[20];    //时间
    char Retriever_id[10];  //retriever的id
    char Retriever_battery[10];  //retriever的电量
    char pup_id[10];  //pup的id
    char pup_signal[10];  //pup的信号
    char pup_battery[10];  //pup的电量
    char pup_aisle[5];  //pup的数据个数
    char pup_A0[10];  
    char pup_A1[10];
    char pup_A2[10];
    char pup_B0[10];
    char pup_B1[10];
    char pup_B2[10];
    char pup_C0[10];
    char pup_C1[10];
    char pup_C2[10];
    char pup_D0[10];
    char pup_D1[10];
    char pup_D2[10];
}PUP;


/*************************************************************
 *retriever数据处理函数
 *参数1:待处理的数据,(字符串)
 *参数2:处理好的数据,(结构体)
 *参数3:
 *返回值:成功返回0,
 
 *       失败返回-1,
 * **********************************************************/
int Reteiever_Processing(char *data,PUP *pup_data);




int dataCommand(char *data);

char **dataJRESS(char sep,const char *str,int *size);

int check_data(char *data);

#endif

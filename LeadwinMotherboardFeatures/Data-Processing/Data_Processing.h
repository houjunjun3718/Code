/*************************************************************************
	> File Name: Data_Processing.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月19日 星期三 14时28分28秒
 ************************************************************************/

#ifndef _DATA_PROCESSING_H
#define _DATA_PROCESSING_H


typedef struct pup
{
    char P_0[50];
    char P_1[50];
    char P_2[50];
    char P_3[50];
    char P_4[50];
    char P_5[50];
    char P_6[50];
    char P_7[50];
    char P_8[50];
    char P_9[50];
    char P_A[50];
    char P_B[50];
    char P_C[50];
    char P_D[50];
    char P_E[50];
    char P_F[50];
    
}PUP;

/*Retriever数据校验函数
 * 参数1:需要校验的数据
 * 返回值:成功返回0
 *        失败返回-1*/
int Retriever_tesr(char *data);

/*Retriever数据处理函数
* 参数1,原始的Retriever数据
* 参数2,用于返回的拆分开从新组合的小字符串
* 返回值:成功返回小字符串的个数
*        返回0:表示校验失败,
*        失败返回-1*/
int Retriever_Data_Processing(char *data,char **element);

#endif

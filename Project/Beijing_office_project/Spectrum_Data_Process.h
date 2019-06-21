/*************************************************************************
	> File Name: Spectrum_Data_Process.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月20日 星期四 22时16分43秒
 ************************************************************************/

#ifndef _SPECTRUM_DATA_PROCESS_H
#define _SPECTRUM_DATA_PROCESS_H


//Spectrum数据结构体
typedef struct
{
    char P0[50];
    char P1[50];
    char P2[50];
    char P3[50];
    char P4[50];
    char P5[50];
    char P6[50];
    char P7[50];
    char P8[50];
    char P9[50];
    char PA[50];
    char PB[50];
    char PC[50];
    char PD[50];
    char PE[50];
    char PF[50];
}PUP;

/*频谱设备数据处理函数
 * 参数1:原始数据
 * 参数2:返回的处理后的数据
 * 返回值:成功返回处理后的字符的个数
 *        失败返回-1*/
int Spectrum_Data_Process(char *data,PUP *element);
#endif

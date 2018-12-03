/*************************************************************************
	> File Name: ReuseFunction.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2018年11月30日 星期五 09时54分50秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#include"ReuseFunction.h"



/*************************************************************************
 *温度转换函数
 *功能:将华氏温度转换为摄氏温度
 *参数0:华氏温度的字符串
 *参数1:摄氏温度的字符串
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int TemperatureConversionFunction(char *F,char *C)
{
    double ren;
    ren = ((atof(F)-32.0) * 5.0 / 9.0);  //浮点数据强转成整型数据舍弃小数点后面的精度.所以最后的温度只保留小数点后一位
    printf("TemperatureConversionFunction:ren = %.1f\n",ren);
    sprintf(C,"%.1f",ren);  //将数据转换成字符串存放到参数地址.
    return 0;
}



/*************************************************************************
 *数据放大函数
 *功能:PLC无法处理浮点数据,所以在使用中将浮点数据扩到到相应的整数
 *参数0:要扩大的字符串数据
 *参数1:扩大的倍数
 *参数2:扩大后的字符串
 *返回值:成功返回0
 *       失败返回-1
 * **********************************************************************/
int DataAmplificationFunction(char *data,int multiple,char *BigData)
{
    int ren;
    ren = atof(data) * multiple;
    printf("DataAmplificationFunction:ren = %d\n",ren);
    sprintf(BigData,"%d",ren);
}


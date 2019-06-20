/*************************************************************************
	> File Name: Spectrum_Data_Process.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年06月20日 星期四 22时16分43秒
 ************************************************************************/

#ifndef _SPECTRUM_DATA_PROCESS_H
#define _SPECTRUM_DATA_PROCESS_H

/*频谱设备数据处理函数
 * 参数1:原始数据
 * 参数2:返回的处理后的数据
 * 返回值:成功返回处理后的字符的个数
 *        失败返回-1*/
int Spectrum_Data_Process(char *data,char **element);
#endif

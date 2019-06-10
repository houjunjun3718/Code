/*************************************************************************
	> File Name: MEC10_Sensor.h
	> Author: 候军军
	> Mail: 15847703113@163.com
    > 功能: 用于读取MEC10传感器的数据
	> Created Time: 2019年05月22日 星期三 11时46分47秒
 ************************************************************************/

#ifndef _MEC10_SENSOR_H
#define _MEC10_SENSOR_H

/*定义一个用于存放MEC10传感器的数据的结构体
 * 土壤温度,土壤湿度,土壤EC*/
typedef struct {
    double temp;
    double vwc;
    double ec;
}MEC10;



/*定义一个数据采集器,里面包含3个传感器
 * 分别是20cm,40cm,60cm*/




/*发送数据采集命令,一次采集地址
 * 参数1:传感器的地址
 * 参数2:回填的数据
 * 参数3:串口的文件描述符
 * 返回值:0,成功
 *        -1,失败*/
int MEC10_Collection(int Address,MEC10 *Sensor,int fd);






#endif

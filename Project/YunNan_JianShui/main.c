/*************************************************************************
	> File Name: main.c
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月08日 星期一 17时36分32秒
 ************************************************************************/

#include<stdio.h>


typedef struct
{
    //气象站数据
    char Time[20];  //时间
    char AirTemperature[20];  //空气温度
    char AirHumidity[20];  //空气湿度
    char Illumination[20];  //光照
    //土壤温度数据
    char NO1Shed40cmSoilTemperature[20];  //1号棚40cm土壤温度
    char NO1Shed70cmSoilTemperature[20];  //1号棚70cm土壤温度
    char NO2Shed40cmSoilTemperature[20];  //2号棚40cm土壤温度
    char NO2Shed70cmSoilTemperature[20];  //2号棚70cm土壤温度
    char NO3Shed40cmSoilTemperature[20];  //3号棚40cm土壤温度
    char NO3Shed70cmSoilTemperature[20];  //3号棚70cm土壤温度
    char NO4Shed40cmSoilTemperature[20];  //4号棚40cm土壤温度
    char NO4Shed70cmSoilTemperature[20];  //4号棚70cm土壤温度
    //土壤湿度数据
    char NO1Shed40cmSoilMoisture[20];  //1号棚40cm土壤湿度
    char NO1Shed70cmSoilMoisture[20];  //1号棚70cm土壤湿度
    char NO2Shed40cmSoilMoisture[20];  //2号棚40cm土壤湿度
    char NO2Shed70cmSoilMoisture[20];  //2号棚70cm土壤湿度
    char NO3Shed40cmSoilMoisture[20];  //3号棚40cm土壤湿度
    char NO3Shed70cmSoilMoisture[20];  //3号棚70cm土壤湿度
    char NO4Shed40cmSoilMoisture[20];  //4号棚40cm土壤湿度
    char NO4Shed70cmSoilMoidture[20];  //4号棚70cm土壤湿度
    //土壤EC数据
    char NO1Shed40cmSoilEC[20];  //1号棚40cm土壤EC
    char NO1Shed70cmSoilEC[20];  //1号棚70cm土壤EC
    char NO2Shed40cmSoilEC[20];  //2号棚40cm土壤EC
    char NO2Shed70cmSoilEC[20];  //2号棚70cm土壤EC
    char NO3Shed40cmSoilEC[20];  //3号棚40cm土壤EC
    char NO3Shed70cmSoilEC[20];  //3号棚70cm土壤EC
    char NO4Shed40cmSoilEC[20];  //4号棚40cm土壤EC
    char NO4Shed70cmSoilEC[20];  //4号棚70cm土壤EC

}Date;

//需要创建2把锁,一把用于数据库,一把用于Date数据结构体

int main()
{
    //初始化串口(用于接收来自Retriever的数据)

    //初始化数据库(存放采集到的数据)

    //初始化数据上传模块(从数据库中读取数据上传到云平台)

    //初始化LED屏(用于本地的数据展示)

    //初始化看门狗(用于监控数据采集端口是否可以采集到数据)

    //初始化锁,(用于数据库的读写操作)

    //初始化锁,(用于数据结构体的读写操作)

    //启动串口数据采集线程(将采集到的数据存入数据库)

    //启动数据看门狗(监测数据)

    //启动数据处理线程(从数据库中读取数据,发送到云端,本地将数据解析,存入LED屏数据结构体中)

    //启动LED屏显示线程,从结构体读取数据,将数据组织起来发送给网口.




    return 0;
}







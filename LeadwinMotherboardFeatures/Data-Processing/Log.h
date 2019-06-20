/*************************************************************************
	> File Name: Log.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月19日 星期五 09时56分50秒
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

//#include<string.h>
//#include<pthread.h>
//#include<time.h>
//#define LOG "./jianshui.log"

//FILE *Log_Fd = NULL;   //日志的文件流
//pthread_mutex_t Log_lock;  //日志的文件锁


/*初始化日志
 *参数:要打开的日志
 *返回值:成功返回文件流
 *       失败返回NULL;
 *
 *需要先创建一个日志文件.
 * */
FILE *InitLog(const char *log);

/*写日志
 * 参数: str   要写入的日志
 *
 * 返回值:  成功返回写入的字节数,
 *          失败返回-1;*/
int WriteLog(const char *str);

/*关闭日志系统*/
#endif

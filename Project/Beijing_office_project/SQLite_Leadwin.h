/*************************************************************************
	> File Name: SQLite_Leadwin.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年02月16日 星期六 14时10分10秒
 ************************************************************************/

#ifndef _SQLITE_LEADWIN_H
#define _SQLITE_LEADWIN_H


/*
 * 初始化sqlite数据库
 * 用于打开已经创建好的数据库
 * 参数db:用于打开数据库
 * 返回值:成功返回用于控制数据库的句柄,失败返回NULL
 * */
sqlite3 * SQLite_Init(chat * db);


/*
 * 用于向数据库插入数据
 * 主要用于数据的通信过程中数据上传失败做备份,等待通信恢复后重新上传数据
 * 参数db:数据库的句柄
 * 参数table:用于插入数据的表.
 * 参数field:用于插入数据的字段.
 * 参数data:用于插入的数据.
 * 返回值:成功返回0,失败返回-1;
 * */
int SQLite_Insert(sqlite3 * db,char * table,char * field,char *data);


/*
 * 找出表中最小的一条数据
 * 用于后期的发送数据和删除数据
 * 参数db:数据库的句柄
 * 参数table:用于查找的数据库
 * 参数field_find:用于查找的字段.
 * 参数field_desplay:用于显示的字段.
 * 返回值:成功返回查询到的数据,失败返回NULL
 *
 *
 * */

#endif

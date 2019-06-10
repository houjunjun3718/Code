/*************************************************************************
	> File Name: Sqliet.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月19日 星期五 19时48分30秒
 ************************************************************************/

#ifndef _SQLIET_H
#define _SQLIET_H

#include<sqliet3.h>


/*
 *数据库初始化
 *参数:Database  数据库
 *返回值:成功返回数据库操作符
 *       失败返回NULL
 * */
sqlite3 *InitSqlite(const char *Database);


/*
 *插入数据
 *参数:data 要插入的数据
 *     db   数据库的操作符
 *返回值:成功返回0
 *       失败返回-1
 * */
int InsertSqlite(const char *data,sqlite3 * db);

/*
 *查询数据
 *参数:db  数据库操作符
 *     结构体:存放最小的编号和数据
 *返回值:成功返回0
 *       失败返回-1
 * */
int inquireSqlite();


/*
 *删除数据
 *参数:db 数据库操作符
 *     id 数据id编号
 *返回值:成功返回0
 *       失败返回-1
 * */
int DeleteSqlite();


/*
 *关闭数据库
 *参数: db 数据库操作符
 *返回值:无返回值
 * */
void CloseSqlite();

#endif

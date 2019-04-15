/*************************************************************************
	> File Name: DataLeadWinCloud.h
	> Author: 候军军
	> Mail: 15847703113@163.com
	> Created Time: 2019年04月14日 星期日 16时30分55秒
 ************************************************************************/

#ifndef _DATALEADWINCLOUD_H
#define _DATALEADWINCLOUD_H

/*初始化发送功能
 *参数cloud:云平台的地址.
 *返回值:成功返回0,
 *       失败返回-1.
 * */
int InitDataLeadWinCloud(char *cloud);



/*数据发送
 *参数cloud:云平台地址
 *参数data:上传的数据
 *返回值:成功返回0,
 *       失败返回-1.
 *       网络中断返回1.
 * */
int ComplyDataLeadWinCloud(char *cloud,char *data);

/*清理函数,当不在调用该模块时使用
 * 参数:无
 * 返回值:无
 * */
void ExitDataLeadWinCloud(void);

#endif

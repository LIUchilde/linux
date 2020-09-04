/*
 * 通过mysql的C语言接口实现mysql的基本操作
 *
 * 1.初始化客户端
 *   初始化句柄  mysql__init
 *   连接服务器 mysql_real_connect
 *   设置字符集  mysql_set_character_set
 * 2.执行语句
 *   执行语句 mysql_query
 *   获取结果 mysql_store_result
 *   遍历结果 mysql_num_rows/mysql_num_fields/mysql_fetch_row
 * 3.关闭数据库  mysql_close
 * 4.执行每次函数获取上一次错误原因 mysql_error
 */


#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

int main()
{
	MYSQL *mysql = mysql_init(NULL);//定义并初始化吗mysql操作句柄
	//链接服务器 mysql_real_connect(句柄,主机地址,用户名,密码,数据库名称,端口,套接字文件,标志)
	if(mysql_real_connect(mysql,"127.0.0.1","root","lc3235420",
				"db_order_sys",0,NULL,0)==NULL){
		printf("mysql connect filed:%s\n",mysql_error(mysql));
		return -1; 
	}

	int ret = mysql_set_character_set(mysql,"utf8");//设置客户端字符编码集
	if(ret != 0){
		printf("set character to utf8 failed:%s\n",mysql_error(mysql));
		return -1;
	}

	char* sql_str = "select *from tb_dishes;";
	ret = mysql_query(mysql,sql_str);//执行语句 
	if(ret != 0){
		printf("sql query failed:%s\n",mysql_error(mysql));
		return -1;
	}
	MYSQL_RES *res = mysql_store_result(mysql);//获取执行结果
	if(res == NULL){
		printf("store result failed:%s\n",mysql_error(mysql));
		return -1;
	}

	int row_nums = mysql_num_rows(res);//获取查询行数
	int col_nums = mysql_num_fields(res);//获取结果的列数
	int i = 0,j = 0;
	for(;i<row_nums;i++){
		MYSQL_ROW row = mysql_fetch_row(res);
		for(j = 0;j < col_nums;j++){
			printf("%s ",row[j]);
		}
	printf("\n");
	}

	mysql_free_result(res);//释放结果集
	mysql_close(mysql);//关闭数据库释放资源
  
	return 0;
} 

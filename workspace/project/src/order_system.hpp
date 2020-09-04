/*
 * 基于mysql封装数据对象操作类
 *   1.菜品信息表的操作类
 *   2.顶大信息表的操作类
 * 注意事项：菜品信息/订单信息的通信格式-采用json进行数据传输
 */

#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

#define MYSQL_SERVER "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASSWD "lc3235420"
#define MYSQL_DB "db_order_sys"

namespace _order_sys{
	extern void MysqlDestroy(MYSQL *mysql);
	extern MYSQL *MysqlInit();
	extern bool MysqlQuery(MYSQL *mysql,const std::string &sql);
	//封装mysql数据库的基本操作，作为静态函数
	//1.mysql初始化
	MYSQL * MysqlInit(){
		MYSQL *mysql=NULL;
		//初始化句柄
		mysql = mysql_init(NULL); 
		if(mysql == NULL){
			printf("mysql init error:\n");
			return NULL;
		}
		//连接服务器
		if(mysql_real_connect(mysql,MYSQL_SERVER,MYSQL_USER,MYSQL_PASSWD,MYSQL_DB,0,NULL,0) == NULL){
			printf("connect mysql server filed:%s\n",mysql_error(mysql));
			MysqlDestroy(mysql);
			return NULL;
		}
		//设置客户端字符编码集
		if(mysql_set_character_set(mysql,"utf8") != 0){
			printf("se mysql client character filed:%s\n",mysql_error(mysql));
			MysqlDestroy(mysql);
			return NULL;
		}
		return mysql;
	}

	//2.mysql销毁
	void MysqlDestroy(MYSQL *mysql){
		if(mysql != NULL){
			mysql_close(mysql);
		}
		return;
	}
	//3.mysql执行语句
	bool MysqlQuery(MYSQL *mysql,const std::string &sql){
		int ret = mysql_query(mysql,sql.c_str());
		if(ret != 0){
			printf("sql:[%s] query flied:%s\n",sql.c_str(),mysql_error(mysql));
			return false;
		}
		return true;
	}
	class DishTable{     //操作数据库中菜单数据
		public:
			DishTable(MYSQL *mysql):_mysql(mysql){}
			bool Insert(const Json::Value &dish){
				#define DISH_INSERT "insert into tb_dishes(id,name,price,uptime) values(null,'%s',%d,now());"
				char buf[4096] = {0};
				//sprintf--将多个数据对象按照指定格式进行组织成为字符串放到指定缓冲区
				//dish["name"].asCString()--获取c语言风格的菜品名称
				//dish["price"].asInt()--获取int类型的菜品单价信息
				sprintf(buf,DISH_INSERT, dish["name"].asCString(),dish["price"].asInt());
				if(MysqlQuery(_mysql,buf) == false){
					return false;
				}
				return true;
			}
			bool selectOne(const int dish_id,Json::Value *dish){
				#define DISH_SELECTONE "select name,price,uptime from tb_dishes where id=%d;"
				char buf[4096] = {0};
				sprintf(buf,DISH_SELECTONE,dish_id);
				if(MysqlQuery(_mysql,buf) == false){//执行查询语句 
                   return false;
                }
                MYSQL_RES *res = mysql_store_result(_mysql);//获取结果集
				if(res == NULL){
					printf("store one dish result failed:%s\n",mysql_error(_mysql));
					return false;
				}
				int num_row = mysql_num_rows(res);//获取结果条数
				if(num_row != 1){
					printf("one dish result error\n");
					return false;
				}
				for(int i=0;i<num_row;i++){ //遍历获取每一条数据
					MYSQL_ROW row = mysql_fetch_row(res);
					(*dish)["id"] = dish_id;
					(*dish)["name"] = row[0];
					(*dish)["price"] = std::stoi(row[1]);
					(*dish)["uptime"] = row[2];
				}
				mysql_free_result(res);//销毁结果集
				return true;
            }
			
			bool selectAll(Json::Value *dishes){
				#define DISH_SELECTALL "select id,name,price,uptime from tb_dishes;"
				if(MysqlQuery(_mysql,DISH_SELECTALL) == false){
					return false;
				}
				MYSQL_RES *res = mysql_store_result(_mysql);//获取结果集
				if(res == NULL){
                	printf("store all dishes result failed:%s\n",mysql_error(_mysql));
                    return false;
                }
				int num_rows = mysql_num_rows(res);//获取结果条数
				for(int i=0;i<num_rows;i++){ 
                    MYSQL_ROW row = mysql_fetch_row(res);//遍历获取每一条数据
					Json::Value dish_val;
					dish_val["id"] = std::stoi(row[0]);
                    dish_val["name"] = row[1];
                    dish_val["price"] = std::stoi(row[2]);
                    dish_val["uptime"] = row[3];
					dishes->append(dish_val);//将单个菜品信息添加到数组中返回
                }
			    mysql_free_result(res);//销毁结果集
			    return true;
 		    }
			
			bool update(const Json::Value &dish){
				#define DISH_UPDATE "update tb_dishes set name='%s',price=%d,uptime=now() where id=%d;"
				char buf[4096] = {0};
				sprintf(buf,DISH_UPDATE,dish["name"].asCString(),dish["price"].asInt(),dish["id"].asInt());
				if(MysqlQuery(_mysql,buf) == false){
                    return false;
                }
				return true;
			}
			bool Delete(const int dish_id){
				#define DISH_DELETE "delete from tb_dishes where id=%d;"
				char buf[4096] = {0};
                sprintf(buf,DISH_DELETE,dish_id);
                if(MysqlQuery(_mysql,buf) == false){
                    return false;
                }
                return true;
			}
			~DishTable(){
			}
		private:
			MYSQL *_mysql;
};


	class OrderTable{     //操作数据库中订单数据
		public:
			OrderTable(MYSQL *mysql):_mysql(mysql){
			}
			bool Insert(const Json::Value &order){
				#define ORDER_INSERT " insert into tb_orders(id,table_id,dishes,status,uptime) values(null,%d,'%s',%d,now());"
				char buf[4096] = {0};
				Json::FastWriter writer;
				std::string dishes = writer.write(order["dishes"]);
				sprintf(buf,ORDER_INSERT,order["table_id"].asInt(),dishes.c_str(),order["status"].asInt());
				if(MysqlQuery(_mysql,buf) == false){
					return false;
				} 
				return true;
			}
			bool selectOne(const int order_id,Json::Value *order){
				#define ORDER_SELECTONE " select table_id,dishes,status,uptime from tb_orders where id=%d;"
				char buf[4096] = {0};
				sprintf(buf,ORDER_SELECTONE,order_id); 
				if(MysqlQuery(_mysql,buf) == false){
					return false;
				}
				MYSQL_RES *res = mysql_store_result(_mysql);//获取结果集
				if(res == NULL){
                	printf("store one order result failed:%s\n",mysql_error(_mysql));
                    return false;
                }
				int num_rows = mysql_num_rows(res);//获取结果条数
				if(num_rows != 1){
					printf("one order result failed:%s");
					return false;
				}
				for(int i=0;i<num_rows;i++){ 
                    MYSQL_ROW row = mysql_fetch_row(res);//遍历获取每一条数据
					(*order)["id"]=order_id;
					(*order)["table_id"]=std::stoi(row[0]);
					Json::Value dishes;
					Json::Reader reader;
					reader.parse(row[1],dishes);//菜品信息的json数组对象
					(*order)["dishes"]=dishes;
					(*order)["status"]=std::stoi(row[2]);
					(*order)["uptime"]=row[3];
				}
				mysql_free_result(res);//销毁结果集
				return true;
			}
			bool selectAll(Json::Value *orders){
				#define ORDER_SELECTALL " select id,table_id,dishes,status,uptime from tb_orders;"
				if(MysqlQuery(_mysql,ORDER_SELECTALL) == false){
					return false;
				}
				MYSQL_RES *res = mysql_store_result(_mysql);//获取结果集
				if(res == NULL){
                	printf("store all orders result failed:%s\n",mysql_error(_mysql));
                    return false;
                }
				int num_rows = mysql_num_rows(res);//获取结果条数
				for(int i=0;i<num_rows;i++){ 
                    MYSQL_ROW row = mysql_fetch_row(res);//遍历获取每一条数据
					Json::Value order_val;
					order_val["id"]=std::stoi(row[0]);
					order_val["table_id"]=std::stoi(row[1]);
					Json::Value dishes;
					Json::Reader reader;
					reader.parse(row[2],dishes);//菜品信息的json数组对象
					order_val["dishes"]=dishes;
					order_val["status"]=std::stoi(row[3]);
					order_val["uptime"]=row[4];
					orders->append(order_val);
				}
				mysql_free_result(res);//销毁结果集
				return true;
			}
			bool update(const Json::Value &order){
				#define ORDER_UPDATE "update tb_orders set table_id=%d,dishes='%s',status=%d, uptime=now() where id=%d;"
				Json::Value dishes = order["dishes"];
				Json::FastWriter writer;
				std::string str_dishes = writer.write(dishes);
				char buf[4096] = {0};
				sprintf(buf,ORDER_UPDATE,order["table_id"].asInt(),str_dishes.c_str(),order["ststus"].asInt(),order["id"].asInt());
				if(MysqlQuery(_mysql,buf) == false){
                    return false;
                }
				return true;
			}
			bool Delete(const int order_id){
				#define ORDER_DELETE "delete from tb_orders where id=%d;"
				char buf[4096] = {0};
                sprintf(buf,ORDER_DELETE,order_id);
                if(MysqlQuery(_mysql,buf) == false){
                    return false; 
                }
                return true;
			}
			~OrderTable(){ 

			}
		private:
			MYSQL*_mysql;
};
}

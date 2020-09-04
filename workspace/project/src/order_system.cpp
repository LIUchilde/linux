#include"order_system.hpp"
#include"httplib.h"

_order_sys::DishTable *dish_table;
_order_sys::OrderTable *order_table;
void InsertDish(const httplib::Request &req,httplib::Response &rsp){
	//req.body 中保存的正文--正文就是菜品信息的json数据串
	Json::Value dish_val;

	Json::Reader reader;
	//reader.parse(json数据串，解析后的Json::Value对象);
	bool ret = reader.parse(req.body,dish_val);
	if(ret == false){
		std::cout << "InsertDish parse json data failed!!\n";
		rsp.status = 400;
		return;
	}
	ret = dish_table->Insert(dish_val);
	if(ret == false){	
		rsp.status = 500;
		return;
	}
	rsp.status = 200;
	return;		
}
void DeleteDish(const httplib::Request &req,httplib::Response &rsp){
	int dish_id = std::stoi(req.matches[1]);//matches中保存所有匹配的字符串
	bool ret = dish_table->Delete(dish_id);
	if(ret == false){	
		rsp.status = 500;
		return;
	}
	rsp.status = 200;
	return;
}
void UpdateDish(const httplib::Request &req,httplib::Response &rsp){
	//	新的菜品信息保存的请求的正文中---req.body
	Json::Value dish_val;
	Json::Reader reader;
	bool ret = reader.parse(req.body,dish_val);
	if(ret == false){
		rsp.status = 400;
		return;
	}
	//更新数据库信息前，应该对信息进行服务器端校验
	ret = dish_table->update(dish_val);
	if(ret == false){	
		rsp.status = 500;
		return;
	}
	rsp.status = 200;
	return;
}
void SelectOneDish(const httplib::Request &req,httplib::Response &rsp){
	int dish_id = std::stoi(req.matches[1]);
	//从数据库中获取指定id的菜品信息
	Json::Value dish_val;
	bool ret = dish_table->selectOne(dish_id,&dish_val);
	if(ret == false){	
		rsp.status = 500;
		return;
	}
	//获取成功，需要将dish_val中的数据对象序列化为json数据串，作为rsp.body
	Json::FastWriter writer;
	std::string body = writer.write(dish_val);
	//正文就是菜品信息的json数据串
	rsp.set_content(body.c_str(),body.size(),"application/json");
	rsp.status = 200;
	return;
}

void SelectAllDish(const httplib::Request &req,httplib::Response &rsp){
	std::cout << "into select all dish\n";
	Json::Value dishes_val;
	bool ret = dish_table->selectAll(&dishes_val);
	if(ret == false){	
		rsp.status = 500;
		return;
	}
	Json::FastWriter writer;
	std::string body;
	body = writer.write(dishes_val);
	rsp.set_content(body.c_str(),body.size(),"application/json");
	rsp.status = 200;
	std::cout << "over select all dish\n";
	return;
}	
void InsertOrder(const httplib::Request &req,httplib::Response &rsp){
	Json::Value order_val;
	Json::Reader reader;
	bool ret = reader.parse(req.body,order_val);
	if(ret == false){
		std::cout << "InsertOrder parse json data failed!!\n";
		rsp.status = 400;
		return;
	}
	ret = order_table->Insert(order_val);
	if(ret == false){	
		std::cout << "InsertOrder inorder data to db failed!!\n";
		rsp.status = 500;
		return;
	}
	rsp.status = 200;
	return;		
	

	return;
}
void DeleteOrder(const httplib::Request &req,httplib::Response &rsp){
	int order_id = std::stoi(req.matches[1]);
	bool ret = order_table->Delete(order_id);
	if(ret == false){	
		std::cout << "DeleteOrder delete order failed!!\n";
		rsp.status = 500;
		return;
	}
	rsp.status = 200;
	return;
}
void UpdateOrder(const httplib::Request &req,httplib::Response &rsp){
	Json::Value order_val;
	Json::Reader reader;
	bool ret = reader.parse(req.body,order_val);
	if(ret == false){
		std::cout << "UpdateOrder parse json data failed!!\n";	
		rsp.status = 400;
		return;
	}
	//更新数据库信息前，应该对信息进行服务器端校验
	ret = order_table->update(order_val);
	if(ret == false){	
		std::cout << "UpdateOrder update data to db failed!!\n";
		return;	
	}
	rsp.status = 200;
	return;
}
void SelectOneOrder(const httplib::Request &req,httplib::Response &rsp){
	int order_id = std::stoi(req.matches[1]);
	Json::Value order_val;
	bool ret = order_table->selectOne(order_id,&order_val);
	if(ret == false){	
		std::cout<<"selectOneOrder get order info from db fsiled\n";
		return ;
	}
	Json::FastWriter writer;
	std::string body;
	body = writer.write(order_val);
	rsp.set_content(body.c_str(),body.size(),"application/json");
	rsp.status = 200;
	return;
}
void SelectAllOrder(const httplib::Request &req,httplib::Response &rsp){	
	Json::Value order_val;
	bool ret = order_table->selectAll(&order_val);
	if(ret == false){	
		std::cout<<"selectAllOrder get order info from db fsiled\n";
		return;
	}
	Json::FastWriter writer;
	std::string body;
	body = writer.write(order_val);
	rsp.set_content(body.c_str(),body.size(),"application/json");
	rsp.status = 200;
	return;
}

#define WWWROOT "./wwwroot" 
int main()
{

	MYSQL *mysql = _order_sys::MysqlInit();
	dish_table = new _order_sys::DishTable(mysql);
	order_table = new _order_sys::OrderTable(mysql);

	httplib::Server server;
	//server.set_base_dir(静态资源默认根目录)
	//设置静态资源根目录之后，如果浏览器请求的是根目录下的资源，httplib就会自动的将资源读取出来返回给浏览器
	server.set_base_dir(WWWROOT);
	//插入菜品请求
	server.Post("/dish",InsertDish);
	//删除菜品请求
	server.Delete(R"(/dish/(\d+))",DeleteDish);
	//更新菜品请求
	server.Put("/dish",UpdateDish);
	//查询单个菜品信息请求
	server.Get(R"(/dish/(\d+))",SelectOneDish);//R"(....)"--使用原始字符串，去除特殊字符串中的特殊含义
	//查询所有菜品信息请求
	server.Get("/dish",SelectAllDish);
	

	//插入订单请求
	server.Post("/order",InsertOrder);
	//删除订单请求
	server.Delete(R"(/order/(\d+))",DeleteOrder);
	//更新订单请求
	server.Put("/order",UpdateOrder);
	//查询单个订单信息请求
	server.Get(R"(/order/(\d+))",SelectOneOrder);
	//查询所有订单信息请求
	server.Get("/order",SelectAllOrder);

	
	server.listen("0.0.0.0",9000);
	_order_sys::MysqlDestroy(mysql);
	return 0;
}

#include"order_system.hpp"
int main()
{
	MYSQL *mysql = _order_sys::MysqlInit();
	if(mysql == NULL){
		return -1;
	}
	_order_sys::DishTable dish_tb(mysql);
	_order_sys::OrderTable order_tb(mysql);
 /* 插入菜品
	Json::Value dish;
	dish["name"] = "盐煎肉";
	dish["price"] = 2400;
	dish_tb.Insert(dish);
*/
/*查询单个菜品信息
	Json::Value dish1;
	Json::StyledWriter writer;
	dish_tb.selectOne(9,&dish1);
	std::cout<<"dish:"<<writer.write(dish1);
*/
/*查看所有菜品信息
	Json::Value dish2;
	Json::StyledWriter writer2;
	dish_tb.selectAll(&dish2);
	std::cout<<"dish:"<<writer2.write(dish2);
*/
/* 更新菜品信息
	Json::Value dish3;
	dish3["id"] = 12;
	dish3["name"] = "小酥肉";
	dish3["price"] = 2700;
	dish_tb.update(dish3);
*/
/*删除菜品信息
	dish_tb.Delete(10);
*/



	Json::Value order;
	Json::StyledWriter writer;
/*插入订单信息	
	order["table_id"]=5;
	order["dishes"].append(9);
	order["dishes"].append(11);
	order["dishes"].append(12);
	order["statue"]=0;
	order_tb.Insert(order);

	Json::Value order1;
	order_tb.selectOne(1,&order1);
	std::cout<<"order:"<<writer.write(order1);

*/
/*获取所有订单信息
	order_tb.selectAll(&order);
	std::cout<<"order:"<<writer.write(order);
*/
/*  修改订单信息
	order["id"]=1;
	order["table_id"]=3;
	order["dishes"].append(9);
	order["statue"]=0;
	order_tb.update(order);
*/
/*删除订单
	order_tb.Delete(1);
*/
	_order_sys::MysqlDestroy(mysql);
	return 0;
}

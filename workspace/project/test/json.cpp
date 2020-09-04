/*
 * 这个demo演示jsoncpp的基本使用:序列化和反序列化的过程
 */

#include<cstdio>
#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>


int main(){
	std::string name = "张三";
	std::string sex = "男";
	int age = 23;

	Json::Value value;//json容器对象，可以直接通过key或val赋值
	value["姓名"] = name;
	value["性别"] = sex;
	value["年龄"] = age;
	value["成绩"].append(88);
	value["成绩"].append(77);
	value["成绩"].append(66);
	
	Json::StyledWriter writer; //json序列化对象
	std::string json_str = writer.write(value);//StyledWriter.write将json::value对象序列化
	printf("StyleWriter json_str:[%s]\n",json_str.c_str());

	Json::FastWriter fwriter;//json序列化对象
    json_str = fwriter.write(value);
	printf("FastWriter json_str:[%s]\n",json_str.c_str());
	//FastWriter相较于StyleWriter：FastWriter序列化的数据串阅读没有StyleWriter强

	Json::Value val2;
	Json::Reader reader;//json反序列化对象，可以实现将json数据串解析成为各个数据对象
	reader.parse(json_str,val2);//将数据串进行解析，得到数据对象放到json::value中
	std::cout << "name:" << val2["姓名"] << std::endl;
	std::cout << "sex:" << val2["性别"] << std::endl;
	std::cout << "age:" << val2["年龄"].asInt() << std::endl;
	std::cout<<"score:" << val2["成绩"] << std::endl;
	for(auto it:val2["成绩"]){//因为成绩对象是一个数组，所以遍历获得其内容
		std::cout << "ch score " << it <<std::endl;	
	}
	return 0;
}

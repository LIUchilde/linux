#include<iostream>
#include "httplib.h"
 
void HelloWorld(const httplib::Request &req,httplib::Response &rsp)
{
   //req----接收到并解析完毕的请求信息--根据这个信息进行业务处理
   //res --- 要求用户进行填充的响应信息
   std::cout << "method:" << req.method << std::endl;
   std::cout <<"path:" <<req.path <<std::endl;
   std::cout <<"body:" <<req.body <<std::endl;
   rsp.status = 200;// 响应状态码
   rsp.body = "<html><body><h1>HelloWorld</h1></body></html>";
   rsp.set_header("Content-Type","text/html");

   //std::string body = "<html><body><h1>HelloWorld</h1></body></html>";
   //rsp.set_content(body,body.size(),"text/html");
   return;
}

int main()
{
	int a=100;
	httplib::Server server;
	//server.Get(请求的资源路径，回调函数地址)
	//针对指定资源的Get请求，采用指定的回调函数
	//server.Get(请求的资源路径，回调函数地址)
    //针对指定资源的Get请求，采用指定的回调函数
    //
    //这些接口相当于添加了一张路由表--什么请求对应什么处理函数
    //请求来了，就从路由表中查看有没有这个请求的处理函数--有就回调/没有就返回404
	server.Get("/helloworld",HelloWorld);
	server.Get("/hi",[&](const httplib::Request &req,httplib::Response &rsp){

    		std::cout<<"a"<<a<<std::endl;
			std::string body = "<html><body><h1>hi</h1></body></html>";
    		rsp.set_content(body.c_str(),body.size(),"text/html");
			});
	server.listen("0.0.0.0",9000);//搭建http服务器
	return 0;
}

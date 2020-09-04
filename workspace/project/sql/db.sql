CREATE DATABASE IF NOT EXISTS db_order_sys;

USE db_order_sys;

CREATE TABLE IF NOT EXISTS tb_dishes(\
id int primary key auto_increment comment"菜品ID",\
name varchar(32) comment"菜品名称",\
price int comment "菜品单价,单位：分",\
uptime datetime comment "菜品修改时间"
);

INSERT INTO tb_dishes VALUES(NULL,"红烧肉",1500,now());
INSERT INTO tb_dishes VALUES(NULL,"土豆丝",1000,now());
INSERT INTO tb_dishes VALUES(NULL,"锅包肉",1800,now());

CREATE TABLE IF NOT EXISTS tb_orders(\
id int primary key auto_increment comment "订单ID",\
table_id int comment"订单桌号",\
dishes varchar(32) comment "保存当前订单的菜品ID",\
status tinyint comment "订单状态0-已完成/1-未完成",\
uptime datetime
); 

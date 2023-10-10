##  mysql表的操作

#  表结构的操作



表的操作主要是学习ddl语言。

##  创建表

![image-20231010154038395](E:\markdown\图片\image-20231010154038395.png)

 

![image-20231010154149877](E:\markdown\图片\image-20231010154149877.png)

##  查看表结构desc

有点像Linux的stay命令。



![image-20231010154308908](E:\markdown\图片\image-20231010154308908.png)

为什么没有显示comment字段。

##  查看历史表的创建语句

show create table user;

![image-20231010154500990](E:\markdown\图片\image-20231010154500990.png)

里面的横线是mysql的分隔符。不想要分隔符，可以用命令。

**show create table users \G**



![image-20231010160135402](E:\markdown\图片\image-20231010160135402.png)

##  使用InnoDB存储引擎建表

这个引擎只会产生两个文件。

![image-20231010161139555](E:\markdown\图片\image-20231010161139555.png)

这就是物理存储结构，实际被mysqld服务转化为一张表的逻辑结构。

  ##  修改表结构

修改也就三种：增加，修改，删除。

新增就是在原来的基础上增加一列。

新增一列属性：

alter table users add iimage_path varchar(128) comment '图片路径'；

![image-20231010164621873](E:\markdown\图片\image-20231010164621873.png)

修改字段的长度：

alter table users modify name varchar(60);

![image-20231010170615077](E:\markdown\图片\image-20231010170615077.png)

然后会发现字段备注没了。

所以未来想要调一下表结构，需要保证之前字段保留下来。

![image-20231010171455336](E:\markdown\图片\image-20231010171455336.png)

##  删表的一列

删表的结构一定要小心

alter table users drop id;

把id一列删除掉。

   

##  修改表的某一列的名字

alter table users change image_path  newpath charchar(128) comment '图像路径';

![image-20231010172609958](E:\markdown\图片\image-20231010172609958.png)

##  对表名字的修改rename

alter table users rename user_set; 

 ##  删除表

drop table users;

![image-20231010173234598](E:\markdown\图片\image-20231010173234598.png)

#  数据类型


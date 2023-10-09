##  mysql基础

##  sql的分类

DDL，DML，DCL虽然都是sql语句，但是也有分类。

ddl是数据定义语言：存数据，需要先有数据库，所以你需要**创建，删除，修改**数据库。不涉及到任何用户数据，而是涉及到数据库的**结构**操作。

dml是数据操纵语言：是对数据的进行操作，对**数据**进行**增加，删除，修改**。

dcl是数据控制语言：主要负责数据库的安全和用户管理工作

![image-20231009194304186](E:\markdown\图片\image-20231009194304186.png)

##  存储引擎

![image-20231009194331854](E:\markdown\图片\image-20231009194331854.png)

##  数据库操作

每一个数据库在Linux下实际是一个目录文件。

显示数据库： show databases;记得加s

删除数据库：drop database + 数据库名

创建数据库：create database +数据库名

 显示创建数据库的细节：show create database +数据库名;

![image-20231009200946987](E:\markdown\图片\image-20231009200946987.png)

加不加反引号，(反引号就是esc键下面的)，取决于数据库名字和系统的关键字是否冲突。

##  操作总结

1.mysql大小写不敏感，推荐小写。

2.建库和建表的时候名称 建议都带上反引号。

##  字符集和校验规则

mysql数据存储和取出来的时候的规则要是一样的。

![image-20231009202706798](E:\markdown\图片\image-20231009202706798.png)

###  字符集



一份数据在被存的是时候有编码，比如gbk。这时候就由字符集来保证。

查看整个系统支持的编码集：show variables like 'character_set_database';

  ![image-20231009202049035](E:\markdown\图片\image-20231009202049035.png)

模糊匹配查询所有支持的编码集：show variables like 'character_set_%';

![image-20231009202206384](E:\markdown\图片\image-20231009202206384.png)

一般字符集和校验规则 需要是匹配的。

 ###  校验集



查看数据库的校验集：show variables like 'collation_database';

![image-20231009202618607](E:\markdown\图片\image-20231009202618607.png)

![image-20231009203131014](E:\markdown\图片\image-20231009203131014.png)

字符集和校验规则要匹配，否则就不行。

查看所有的校验集：show collation；

![image-20231009203246222](E:\markdown\图片\image-20231009203246222.png)

##  创建数据库+字符集+编码集 

创建数据库可以指定字符集和编码集。

如果不指明，则用默认的字符集和编码集。

指明确定的字符集：
create database online_d2 character set gbk;//指定字符集为gbk

第二种写法：

create database online_d3 charset=gbk;

###  创建数据库+字符集+编码集

create database online_d4 charset=utf8 collate utf8_bin;

![image-20231009222004944](E:\markdown\图片\image-20231009222004944.png)

**通过查看创建的数据库的db.opt文件可以查看数据库的字符集和校验规则**

![image-20231009222140878](E:\markdown\图片\image-20231009222140878.png)

##  未来创建数据规则

尽量不要指定字符集和校验规则。

##  设置数据库的编码和校验规则对什么影响？

![image-20231009223450364](E:\markdown\图片\image-20231009223450364.png)

本质是对数据库内部的表的编码和校验规则。

假如使用gc_db的话是忽略大小写的，查询的时候可以查出大小写。

![image-20231009224319126](E:\markdown\图片\image-20231009224319126.png)

utf8_bin是区分大小写的。所以使用bin_db数据库是按照以下规则实现的。

![image-20231009224721181](E:\markdown\图片\image-20231009224721181.png)

##  进入数据库

选择使用数据库：use + 数据库名；

然后创建表，这个表的结构的编码和校验规则就是继承自数据库的。

查看数据库中存在的表：show tables；

select database()可以查看自己在哪个数据库中，相当于linux中的pwd。

##  order by排序

select *from person order by name;

![image-20231009225015454](E:\markdown\图片\image-20231009225015454.png)

二者校验规则不同，排序不同，因为gc_db不区分大小写。



![image-20231009225216928](E:\markdown\图片\image-20231009225216928.png)

##  

##  删除数据库

级联就是递归。数据库轻易不要删

![image-20231009225758548](E:\markdown\图片\image-20231009225758548.png)

##  修改数据库alter database

更改数据库的字符集：alter database gc_db charset=gbk;

更改字符集和校验集：

![image-20231009230341498](E:\markdown\图片\image-20231009230341498.png)

##  数据库的备份(不要直接cp命令备份)

**数据库备份是将历史上所有sql语句做了备份。**

因为sql语句里有了数据。所以就这样备份就行。

csq@ubuntu:~$ which mysqldump

/usr/bin/mysqldump

csq@ubuntu:~$ mysqldump -P3306 -u root -p -B bin_db > bin_db.sql

-B选项是选择备份的数据库名字。然后重定向到自己的文件中。

##  数据库的恢复

在mysql中输入source /root/mysql/bin_db.sql即可恢复

##  查看登录数据库的用户

show processlist;

![image-20231009233224614](E:\markdown\图片\image-20231009233224614.png)
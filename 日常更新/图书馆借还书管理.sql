#select * from reader;
#desc reader;
#alter table reader modify  typeno char(1) default '1';
#alter table readertype add check (max_quantity <= 30);
#insert into  readertype values('1','马可波罗', 20, 2);
#insert into  readertype values('4','钢铁', 40, 3);
#alter table reader add check (sex in("男", "女"));
#insert into reader values(1, "小刚","人", '1', '1', '176131');
#alter table book add check (price > 0);

#insert into book values ("00539040","大学物理辅导","吕金钟","清华大学出版社","2020-03-01",-10,18,"一层C-18-4");
#insert into  test1 values(1, '小米');
#alter table borrow add foreign key(rid) references reader(rid);
#desc borrow;

#alter table borrow add foreign key(bid) references book(bid);
#show create table borrow;

#select * from readertype;
#delete from readertype;
#insert into readertype values("1", "教师", 20, 90);
#insert into readertype values("2", "学生", 15, 60);

/*insert into reader values("10100391","杨丽","女","1","外国语学院","92331458"),
("10501561","赵晓丽","女","1","经济与管理学院","92337521"),
("120211010103","宋洪博","男","2","外国语学院","81771211"),
("120211041129","侯明斌","男","2","电气与电子工程学院","81771234"),
("120211070101","李淑子","女","2","数理学院","81775643"),
("120211070106","刘丽","女","2","数理学院","81775644");*/


#delete from reader where(rid = 1);  

/*insert into book values("00551060","PLC应用技术","黄中玉","人民邮电出版社","2018-09-01",42.00,10,"一层E-12-5"),
("00632333","数学分析习题演练","周民强","科学出版社","2020-01-01",65.00,10,"一层J-6-1"),
("00868171","物理学中的群论基础","徐建军","清华大学出版社","2010-09-01",287.80,3,"一层D-5-6"),
("01059432","FPGA设计","张义和","科学出版社","2013-07-01",52.00,9,"一层D-1-3"),
("01086319","空间信息数据库","牛新征","人民邮电出版社","2014-04-01",65.00,6,"二层A-4-1"),
("01244785","MATLAB科学计算","温正","清华大学出版社","2017-08-01",99.00,4,"二层C-12-2"),
("01257680","SQL进阶教程","MICK","人民邮电出版社","2017-11-01",79.00,8,"二层B-8-2"),
("01315502","MySQL数据库管理实战","甘长春","人民邮电出版社","2019-04-01",99.00,8,"二层B-10-1"),
("01331088","大数据技术基础","薛志东","人民邮电出版社","2018-08-01",55.00,12,"二层F-3-4");*/




/*insert into borrow values("10100391","01086319","2022-06-3","2022-7-10"),
("10100391","01331088","2022-06-3",NULL),
("10501561","00868171","2022-03-1","2022-4-28"),
("10501561","01086319","2022-03-1","2022-4-28"),
("10501561","01244785","2022-06-30",NULL),
("10501561","01315502","2022-03-18","2022-4-28"),
("10501561","01331088","2022-06-30",NULL),
("120211010103","00539040","2022-03-15","2022-04-27"),
("120211010103","00632333","2022-03-15","2022-04-27"),
("120211010103","01315502","2022-04-10",NULL),
("120211041129","00539040","2022-03-10","2022-5-5"),
("120211041129","00632333","2022-03-10","2022-5-5"),
("120211041129","01257680","2022-07-1",NULL),
("120211041129","01315502","2022-06-22","2022-7-1"),
("120211070101","00632333","2022-04-6","2022-7-3");*/

/*insert into reader values("10631218","李亚明","男","1","控制与计算机工程学院","92331921"),
("10701274","孟凯彦","男","1","数理学院","92336872");*/
#insert into book values("01351006","Access数据库教程","苏林萍","人民邮电出版社","2021-07-01",59.80,10,"二层F-4-6");

#update borrow set borrowtiem = 20220705, returntime = NULL where(rid = 10100391);

#update book set total = total+1 where(bid = 00539040);

#update borrow set returntime =curdate()  where rid = 10100391;
#update book set total = total + 1 where bid = 123456;

#delete from reader where rid = 10701274;
#delete from reader where rid = 10631218;
#delete from book where bid = 01351006;

#select * from book;

#select bid, bname,price*0.7 from book;

#select * from book order by  publishdata desc;
#select * from book 
#limit 3 offset 6;

select distinct rid from borrow;
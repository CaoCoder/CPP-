/*1．系统函数应用
（1）查询学生的院系、班级名称。
院系班级名称是由院系名称deptname和班级classno连接构成的，例如“外国语学院”与“英语2101”使用字符串连接函数CONCAT得到“外国语学院英语2101”。
*/
/*select student.classno, student.sname,concat(department.deptname, student.classno) 院系班级名称 
from student join department on department.deptno = student.deptno*/

/*(2）将“数据库应用”课程的成绩grade开平方根乘以10，并保留1位小数。
*/
/*select student.classno, student.sname,round(sqrt(score.grade)*10,1)  成绩 from score
 join student on score.sno = student.sno
 join course on score.cno = course.cno
where(course.cname = "数据库应用");
*/

/*查询年龄为20岁的学生信息。
采用日期函数表达式“YEAR(NOW())-YEAR(birthdate)”求出年龄。*/
/*select student.sno, student.sname
from student
where year(now())-year(birthdate) = 21;*/

/*查询3月份出生的学生信息。
日期函数MONTH可以求出出生日期birthdate的月份*/

/*select student.sno, student.sname ,student.birthdate
from student
where month(birthdate) = 3;*/


/*存储过程应用
（1） 创建无输入参数和输出参数的存储过程p_count，比较男女生人数。*/

/*create procedure p_count()
select count(sno) 学生人数
from student;*/

-- call p_count();


/*delimiter $$
create procedure p_avggrage(in vcname char(50), out evaluate char(50))
begin
declare vavg float;
	select avg(grade) into vavg
	from course join score on course.cno=score.cno
join student on student.sno=score.sno
where cname=vcname;
	if vavg >=80 THEN
		set evaluate = "该课程成绩达到预期效果";
	ELSE	
		set evaluate = "未达到预期效果";
	end if;
end $$
delimiter;*/

/*drop procedure p_countsex1;*/



-- call p_avggrage("数据库应用", @eval);
select @eval

-- create index I_tname on teacher(tname)

-- create index index_teacber on teacher(deptno, title);

-- create unique index index_teaching on teaching(classno, tno, cno);

/*视图管理*/

/*create view v_grade 
as select score.sno,student.sname,course.cname,score.grade 
from score 
join student on score.sno = student.sno
join course on score.cno = course.cno;
SELECT sname AS 姓名, cname AS 课程名称,grade AS 成绩
   FROM v_grade WHERE sname="李华";*/
	 
	 
/*创建名为v_count的视图，查询每门课程的选课人数。
创建视图v_count的语句*/

/*create view v2_count 
as select score.sno, course.cname as 课程名称
from course join score on course.cno = score.cno*/

/*create view v_hours as
select course.cno, course.cname, course.hours from course*/

/*insert into v_hours values(19999999,"大学计算机基础",32);*/
delete from v_hours
where cname="大学计算机基础"

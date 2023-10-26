/*select distinct student.sno, sname, enterscore from student, score
 order by enterscore desc
 limit 3;*/
 
 
/*select tno, tname, sex, title, deptname 
from teacher inner join department on teacher.deptno = department.deptno;*/

/*select score.sno 学号, student.sname 姓名,grade 成绩 from score 
inner join course on course.cno = score.cno 
inner join student on student.sno = score.sno
where (score.cno = 10700053) ;*/


/*select student.sno 学号, student.sname 姓名,
course.cname 课程名称, score.grade 成绩
from  student 
inner join score on score.sno = student.sno
inner join course on course.cno = score.cno
where ((course.cname = '证券投资学' or course.cname = '大学物理')and (score.grade < 60));*/


/*select student.sno as 学号,score.sno , student.sname as 姓名
from student
left join score on student.sno = score.sno
where score.sno is null;*/
/*select  
count(student.sno) as 学生人数 
from department
left join student on department.deptno = student.deptno
group by department.deptname;*/

/*select teacher.tname as 姓名, count(teacher.tname) as 班级数量 from teacher
left join teaching on teacher.tno = teaching.tno
group by teacher.tname
order by count(teacher.tname) desc;*/

/*select *from teacher
left join teaching on teacher.tno = teaching.tno;*/






/*select department.deptname as 院系名称, 
sum(score.grade) as 平均分
from depart
*/
/*select department.deptname as 院系名称,avg(score.grade) as 平均成绩  from student
left join  department on  department.deptno = student.deptno
left join  score on score.sno = student.sno
group by department.deptno
order by 平均成绩 desc;*/


/*SELECT d.deptname AS 院系名称, AVG(s.grade) AS 平均成绩
FROM department d
LEFT JOIN student st ON d.deptno = st.deptno
LEFT JOIN score s ON st.sno = s.sno
GROUP BY d.deptname
ORDER BY 平均成绩 DESC;*/

#select * from student;
/*select course.cname as 课程名称, count(score.cno) as 人数
from score
left join course on course.cno  = score.cno
group by score.cno
order by 人数 desc;*/

/*select course.cno as 课程编号, course.cname as 课程名称,
avg(score.grade) as 平均分
from score
left join course on course.cno  = score.cno
group by score.cno*/


select student.sno as 学号, student.sname as 姓名,
count()

// intern.h
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <windows.h>
#include <unordered_set>
#include <iomanip> // 用于设置输出格式
#include <numeric> // 用于计算平均值
using namespace std;
// 实习生每个月的信息
struct Intern {
    string name;               // 实习生姓名
    int id;                    // 实习生ID号
    string school;             // 实习生学校
    double baseSalary;         // 基本工资
    double positionSalary;     // 职务工资
    double allowance;          // 岗位津贴
    double medicalInsurance;   // 医疗保险
    int internshipDays;        // 实习天数
};

// 月度报告结构体
struct MonthlyInfo {
    int month;                // 月份
    int year;                 // 年份

    Intern intern;  // 某个实习生某个月的信息
}monthlyInfo;


// 数据存储容器
extern vector<MonthlyInfo> monthlyInfos;

// 函数声明
void addIntern();                      // 添加实习生
void modifyIntern();                   // 修改实习生信息
void searchIntern();                   // 查询实习生信息
void deleteIntern();                   // 删除实习生信息
   
void printAllInterns();                // 打印所有实习生信息
void saveDataToFile();                 // 保存数据到文件
void loadDataFromFile();               //   加载读取文件
// 函数声明
void searchInternByIdOrName();      
void calculateAverageSalaryBySchool();


void sortByMonth();
void calculateAverageSalaryForAll();


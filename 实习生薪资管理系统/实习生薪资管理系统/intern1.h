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
#include <iomanip> // �������������ʽ
#include <numeric> // ���ڼ���ƽ��ֵ
using namespace std;
// ʵϰ��ÿ���µ���Ϣ
struct Intern {
    string name;               // ʵϰ������
    int id;                    // ʵϰ��ID��
    string school;             // ʵϰ��ѧУ
    double baseSalary;         // ��������
    double positionSalary;     // ְ����
    double allowance;          // ��λ����
    double medicalInsurance;   // ҽ�Ʊ���
    int internshipDays;        // ʵϰ����
};

// �¶ȱ���ṹ��
struct MonthlyInfo {
    int month;                // �·�
    int year;                 // ���

    Intern intern;  // ĳ��ʵϰ��ĳ���µ���Ϣ
}monthlyInfo;


// ���ݴ洢����
extern vector<MonthlyInfo> monthlyInfos;

// ��������
void addIntern();                      // ���ʵϰ��
void modifyIntern();                   // �޸�ʵϰ����Ϣ
void searchIntern();                   // ��ѯʵϰ����Ϣ
void deleteIntern();                   // ɾ��ʵϰ����Ϣ
   
void printAllInterns();                // ��ӡ����ʵϰ����Ϣ
void saveDataToFile();                 // �������ݵ��ļ�
void loadDataFromFile();               //   ���ض�ȡ�ļ�
// ��������
void searchInternByIdOrName();      
void calculateAverageSalaryBySchool();


void sortByMonth();
void calculateAverageSalaryForAll();


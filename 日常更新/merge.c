#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>

void displayScientistInfo(int year, int month) {
    // 钱学森的生平事迹信息
    if (year >= 1929 && year <= 1934) {
        printf("钱学森1929-1934年就读于国立交通大学机械工程系。\n");
    }
    else if (year == 1939) {
        printf("钱学森1939年获得美国加州理工学院航空和数学博士学位。\n");
    }
    else if (year == 1947) {
        printf("钱学森1947年任麻省理工学院教授。\n");
    }
    else if (year == 1956) {
        printf("钱学森1956年任中国科学院力学研究所所长。\n");
    }
    else if (year == 1957) {
        printf("钱学森1957年补选为中国科学院学部委员(院士)。\n");
    }
    else if (year == 1965) {
        printf("钱学森1965年任中华人民共和国第七机械工业部副部长。\n");
    }
    else if (year == 1970) {
        printf("钱学森1970年任中国人民解放军国防科学技术委员会副主任。\n");
    }
    else if (year == 1986 && month == 6){
        printf("钱学森1986年6月任中国科学技术协会主席。\n");
    }
    else if (year == 1994) {
        printf("钱学森1994年当选为中国工程院院士。\n");
    }
    else if (year == 1991 && month == 10) {
        printf("钱学森被国务院、中央军委授予“国家杰出贡献科学家”荣誉称号，被中央军委授予一级英雄模范奖章。\n");
    }
    else
    {
        printf("该日期没有生平事迹,请重新输入\n");
    }
}

// 输出月历表
void displayCalendar(int year, int month) {
    int days, i, week;
    if (month == 0)
    {
        printf("该生平事迹只有年份，没有具体月份,故不显示日历\n\n");
        return;
    }
    // 计算该月份的天数
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            days = 29;
        }
        else {
            days = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        days = 30;
    }
    else {
        days = 31;
    }

    // 计算该月份第一天是星期几
    week = 1;  
    for (i = 1900; i < year; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) {
            week += 366 % 7;
        }
        else {
            week += 365 % 7;
        }
        week %= 7;
    }
    for (i = 1; i < month; i++) {
        if (i == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                week += 29 % 7;
            }
            else {
                week += 28 % 7;
            }
        }
        else if (i == 4 || i == 6 || i == 9 || i == 11) {
            week += 30 % 7;
        }
        else {
            week += 31 % 7;
        }
        week %= 7;
    }

    // 输出月历表
    printf(" Su Mo Tu We Th Fr Sa\n");
    for (i = 0; i < week; i++) {
        printf("   ");
    }
    for (i = 1; i <= days; i++) {
        printf("%3d", i);
        week++;
        if (week % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
    int year, month;

    printf("请输入年份和月份（以空格分隔，月份输入为0，代表月份不详）：");
    scanf("%d %d", &year, &month);

    printf("\n");
    displayCalendar(year, month);
    displayScientistInfo(year, month);

    return 0;
}
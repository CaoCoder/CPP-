#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>

void displayScientistInfo(int year, int month) {
    // Ǯѧɭ����ƽ�¼���Ϣ
    if (year >= 1929 && year <= 1934) {
        printf("Ǯѧɭ1929-1934��Ͷ��ڹ�����ͨ��ѧ��е����ϵ��\n");
    }
    else if (year == 1939) {
        printf("Ǯѧɭ1939��������������ѧԺ���պ���ѧ��ʿѧλ��\n");
    }
    else if (year == 1947) {
        printf("Ǯѧɭ1947������ʡ��ѧԺ���ڡ�\n");
    }
    else if (year == 1956) {
        printf("Ǯѧɭ1956�����й���ѧԺ��ѧ�о���������\n");
    }
    else if (year == 1957) {
        printf("Ǯѧɭ1957�겹ѡΪ�й���ѧԺѧ��ίԱ(Ժʿ)��\n");
    }
    else if (year == 1965) {
        printf("Ǯѧɭ1965�����л����񹲺͹����߻�е��ҵ����������\n");
    }
    else if (year == 1970) {
        printf("Ǯѧɭ1970�����й������ž�������ѧ����ίԱ�ḱ���Ρ�\n");
    }
    else if (year == 1986 && month == 6){
        printf("Ǯѧɭ1986��6�����й���ѧ����Э����ϯ��\n");
    }
    else if (year == 1994) {
        printf("Ǯѧɭ1994�굱ѡΪ�й�����ԺԺʿ��\n");
    }
    else if (year == 1991 && month == 10) {
        printf("Ǯѧɭ������Ժ�������ί���衰���ҽܳ����׿�ѧ�ҡ������ƺţ��������ί����һ��Ӣ��ģ�����¡�\n");
    }
    else
    {
        printf("������û����ƽ�¼�,����������\n");
    }
}

// ���������
void displayCalendar(int year, int month) {
    int days, i, week;
    if (month == 0)
    {
        printf("����ƽ�¼�ֻ����ݣ�û�о����·�,�ʲ���ʾ����\n\n");
        return;
    }
    // ������·ݵ�����
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

    // ������·ݵ�һ�������ڼ�
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

    // ���������
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

    printf("��������ݺ��·ݣ��Կո�ָ����·�����Ϊ0�������·ݲ��꣩��");
    scanf("%d %d", &year, &month);

    printf("\n");
    displayCalendar(year, month);
    displayScientistInfo(year, month);

    return 0;
}
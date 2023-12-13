
#include <vector>

#include <windows.h>
#include <iostream>
#include <thread>
#include <algorithm>
#include <string>
#include <iomanip>
#include <map>
using namespace std;


struct Emp {

    int id;
    double bs;  
    double ps;  
    double al;  
    double la;  
    double ta;  
};


vector<Emp> emps;


void showMenu() {
    printf("***********************************\n");
    printf("***********************************\n");
    printf("*****1. ���ݹ��Ų�ѯ����� ******\n");
    printf("*****2. ���ݹ��Ų�ѯ�ܹ���   ******\n");
    printf("*****3. ���ݹ��Ų�ѯ�������� ******\n");
    printf("*****4. ���ݹ���,�����޸Ĺ���******\n");
    printf("***********************************\n");
    printf("***********************************\n");
}


void modifySalary() {
    int empId;
    printf("�����������빤�ţ�: ");
    cin >> empId;

    size_t i = 0;
    while (i < emps.size() && emps[i].id != empId) {
        ++i;
    }

    if (i < emps.size()) {
        double oldSalary, newSalary;
        printf("�����빤����Ŀ����: ");
        string st;
        cin.ignore();
        getline(cin, st);

        if (st == "��������") {
            oldSalary = emps[i].bs;
            printf("�������µĻ�������: ");
            cin >> newSalary;
            emps[i].bs = newSalary;
        }
        else if (st == "ְ����") {
            oldSalary = emps[i].ps;
            printf("�������µ�ְ����: ");
            cin >> newSalary;
            emps[i].ps = newSalary;
        }
        else if (st == "����") {
            oldSalary = emps[i].al;
            printf("�������µĽ���: ");
            cin >> newSalary;
            emps[i].al = newSalary;
        }
        else if (st == "�����") {
            oldSalary = emps[i].la;
            printf("�������µ������: ");
            cin >> newSalary;
            emps[i].la = newSalary;
        }
        else if (st == "��ͨ����") {
            oldSalary = emps[i].ta;
            printf("�������µĽ�ͨ����: ");
            cin >> newSalary;
            emps[i].ta = newSalary;
        }
        else {
            printf("----------ע�⹤����Ŀ�Ҳ���\n");
            return;
        }

        printf("----------�鿴�޸�ǰ���: %.2f\n\n", oldSalary);
        printf("----------�鿴�޸ĺ���: %.2f\n\n", newSalary);
    }
    else {
        printf("δ�ҵ�����Ϊ %d ��ְ��\n", empId);
    }
}
// ��������Ա����ƽ������
double calcTotalAvg() {
    double totalSalary = 0.0; 
    int count = 0;  
    size_t i = 0;  

    // ��������Ա��
    while (i < emps.size()) {
        
        totalSalary += emps[i].bs + emps[i].ps + emps[i].al + emps[i].la + emps[i].ta;
        count++;  
        i++;  
    }

    // �����Ա�����򷵻�ƽ�����ʷ��򷵻�0
    return (count > 0) ? totalSalary / count : 0.0;
}


double calcAvg(const std::string& type) {
    std::map<std::string, double Emp::* > typeMap = {
        { "��������", &Emp::bs },
        { "ְ����", &Emp::ps },
        { "����", &Emp::al },
        { "�����", &Emp::la },
        { "��ͨ����", &Emp::ta }
    };

    double totalSalary = 0.0;
    int count = 0;

    auto it = typeMap.find(type);
    if (it != typeMap.end()) {
        for (const auto& emp : emps) {
            totalSalary += emp.*(it->second);
            count++;
        }
    }

    return (count > 0) ? totalSalary / count : 0.0;
}

void querySingleSalary()
{
    int empId;
    printf("�����������빤�ţ�: ");
    cin >> empId;

    size_t i = 0;
    while (i < emps.size() && emps[i].id != empId) {
        ++i;
    }

    if (i < emps.size()) {
        double salary;
        printf("�����������빤����Ŀ������: ");
        string st;
        cin.ignore();
        getline(cin, st);

        if (st == "��������") {
            salary = emps[i].bs;
            st = "��������";
        }
        else if (st == "ְ����") {
            salary = emps[i].ps;
            st = "ְ����";
        }
        else if (st == "����") {
            salary = emps[i].al;
            st = "����";
        }
        else if (st == "�����") {
            salary = emps[i].la;
            st = "�����";
        }
        else if (st == "��ͨ����") {
            salary = emps[i].ta;
            st = "��ͨ����";
        }
        else {
            printf("�Ҳ���������Ŀ\n");
            return;
        }

        printf("���ʽ��: %.2f\n", salary);
        double averageSalary = calcAvg(st);
        if (averageSalary > 0.0) {
            printf("������Ŀ %s ��ƽ������Ϊ: %.2f\n", st.c_str(), averageSalary);
        }
        else {
            printf("�Ҳ���������Ŀ\n");
        }
    }
    else {
        printf("----------δ�ҵ�����Ϊ %d ��ְ��\n", empId);
    }
}

void queryts() {
    int empId;
    std::cout << "�����������빤�ţ�: ";
    std::cin >> empId;

    auto it = std::find_if(emps.begin(), emps.end(), [empId](const auto& emp) {
        return emp.id == empId;
        });

    double totalAverageSalary = calcTotalAvg();
    if (totalAverageSalary > 0.0) {
        std::cout << "�������ְ����ƽ���ܹ���Ϊ: " << std::fixed << std::setprecision(2) << totalAverageSalary << std::endl;
    }
    else {
        std::cout << "----------ְ�����ݴ���" << std::endl;
    }
}

double calcTotalRank() 
{
    double ts = 0.0;
    int count = 0;

    printf("***********��鿴���ʱ�**********\n");
    printf("%-6s%-12s%-12s\n", "����", "�ܹ���", "");

    for (size_t i = 0; i < emps.size(); ++i) {
        double employeets = emps[i].bs + emps[i].ps + emps[i].al + emps[i].la + emps[i].ta;
        printf("%-6d%-12.2f%-12s\n", emps[i].id, employeets, "");
        ts += employeets;
        count++;
    }

    return (count > 0) ? ts / count : 0.0;
}

// ��ѯ��������
void querySalaryRank()
{
    int empId;  // Ա������
    printf("�����������빤�ţ�: ");
    cin >> empId;

    // ���ݹ����ܺ�����Ա��
    sort(emps.begin(), emps.end(),
        [](const Emp& a, const Emp& b) {
            return (a.bs + a.ps + a.al + a.la + a.ta) >
                (b.bs + b.ps + b.al + b.la + b.ta);
        });

    size_t i = 0;
    // ��������Ա���б��в���ָ�����ŵ�Ա��
    while (i < emps.size() && emps[i].id != empId)
    {
        ++i;
    }

    if (i < emps.size())
    {
        // ����Ա�����ܹ�������
        int rank = distance(emps.begin(), emps.begin() + i) + 1;
        printf("�ܹ�������: %d\n", rank);
    }
    else
    {
        printf("�Ҳ�������Ϊ %d ��ְ��\n", empId);
    }

    calcTotalRank();
}



int main() {
   
    emps.push_back({ 2301, 1235, 798, 1980, 260, 200 });
    emps.push_back({ 2302, 1165, 648, 1350, 250, 200 });
    emps.push_back({ 2303, 885, 442, 980, 200, 150 });
    emps.push_back({ 2304, 1375, 875, 2100, 280, 200 });
    emps.push_back({ 2305, 773, 396, 840, 200, 150 });
    emps.push_back({ 2306, 1525, 910, 2050, 300, 200 });

    int n;
    do {
        showMenu();
        printf("����������(1-4): ");
        cin >> n;

        switch (n) {
        case 1:
            printf("------1. ���ݹ��Ų�ѯ����� ------\n");
          
            querySingleSalary();
            break;
         
        case 2:
            printf("------2. ���ݹ��Ų�ѯ�ܹ���   ------\n");
        
            queryts();
            break;
        case 3:
            printf("------3. ���ݹ��Ų�ѯ�������� ------\n");
          
            querySalaryRank();
            break;
        case 4:
            printf("------4. ���ݹ���,�����޸Ĺ���------\n");
            modifySalary();
            break;
   
        default:
            printf("------����������뷶Χ��1��4------\n");
            return 0;
        }

        printf("---------------------��ע�⣬��ע��,��ע�⣡ �ȴ�����󷵻����˵���-------------------");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        system("cls");

        printf("\n\n");
    } while (1);

    return 0;
}

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;

const int MAX_EMPLOYEES = 6;
const int MAX_NAME_LENGTH = 20;

struct Employee {
    int id;
    double basicSalary;
    double positionSalary;
    double allowance;
    double livingAllowance;
    double transportAllowance;
};

vector<Employee> employees;
void PrintSalary() {
    cout << "======= ְ��������Ϣ�� =======\n";
    cout << setw(6) << "����" << setw(12) << "��������" << setw(12) << "ְ����"
        << setw(12) << "����" << setw(15) << "�����" << setw(15) << "��ͨ����" << endl;

    for (const auto& emp : employees) {
        cout << setw(6) << emp.id << setw(12) << emp.basicSalary << setw(12) << emp.positionSalary
            << setw(12) << emp.allowance << setw(15) << emp.livingAllowance << setw(15) << emp.transportAllowance << endl;
    }
}
void displayMenu() {
    cout << "======= ���ʹ���ϵͳ =======\n";
    cout << "A. ��ѯ�����\n";
    cout << "B. ��ѯ�ܹ���\n";
    cout << "C. ��ѯ��������\n";
    cout << "D. �޸Ĺ���\n";
    cout << "E. �鿴ְ��������Ϣ��\n";
    cout << "===========================\n";
}

double calculateTotalAverage() {

    double totalSalary = 0.0;
    int count = 0;

    for (const auto& emp : employees) {
        double employeeTotalSalary = emp.basicSalary + emp.positionSalary + emp.allowance + emp.livingAllowance + emp.transportAllowance;
        totalSalary += employeeTotalSalary;
        count++;
    }

    if (count > 0) {
        return totalSalary / count;
    }
    else {
        return 0.0;
    }
}

double calculateAverage(string salaryType) {

    double totalSalary = 0.0;
    int count = 0;

    for (const auto& emp : employees) {
        if (salaryType == "��������") {
            totalSalary += emp.basicSalary;
            count++;
        }
        else if (salaryType == "ְ����") {
            totalSalary += emp.positionSalary;
            count++;
        }
        else if (salaryType == "����") {
            totalSalary += emp.allowance;
            count++;
        }
        else if (salaryType == "�����") {
            totalSalary += emp.livingAllowance;
            count++;
        }
        else if (salaryType == "��ͨ����") {
            totalSalary += emp.transportAllowance;
            count++;
        }
    }

    if (count > 0) {
        return totalSalary / count;
    }
    else {
        return 0.0;
    }
}
void querySingleSalary() {
    int empId;
    cout << "�����빤��: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    if (it != employees.end()) {
        double salary;
        cout << "�����빤����Ŀ���ƣ���������ʡ�ְ���ʵȣ�: ";
        string salaryType;
        cin.ignore();
        getline(cin, salaryType);

        if (salaryType == "��������") {
            salary = it->basicSalary;
            salaryType = "��������";
        }
        else if (salaryType == "ְ����") {
            salary = it->positionSalary;
            salaryType = "ְ����";
        }
        else if (salaryType == "����") {
            salary = it->allowance;
            salaryType = "����";
        }
        else if (salaryType == "�����") {
            salary = it->livingAllowance;
            salaryType = "�����";
        }
        else if (salaryType == "��ͨ����") {
            salary = it->transportAllowance;
            salaryType = "��ͨ����";
        }
        else {
            cout << "��Ч�Ĺ�����Ŀ\n";
            return;
        }

        cout << "���ʽ��: " << fixed << setprecision(2) << salary << endl;
        double averageSalary = calculateAverage(salaryType);
        if (averageSalary > 0.0) {
            std::cout << "������Ŀ " << salaryType << " ��ƽ������Ϊ: " << std::fixed << std::setprecision(2) << averageSalary << std::endl;
        }
        else {
            std::cout << "��Ч�Ĺ�����Ŀ" << std::endl;
        }

        //�������ʵ�����ְ����ƽ������
    }
    else {
        cout << "�Ҳ�������Ϊ " << empId << " ��ְ��\n";
    }
}

void queryTotalSalary() {
    int empId;
    cout << "�����빤��: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    
        double totalAverageSalary = calculateTotalAverage();
        if (totalAverageSalary > 0.0) {
            std::cout << "����ְ����ƽ���ܹ���Ϊ: " << std::fixed << std::setprecision(2) << totalAverageSalary << std::endl;
        }
        else {
            std::cout << "��Ч��ְ������" << std::endl;
        }
   
}
double calculateTotalRank() {

    double totalSalary = 0.0;
    int count = 0;

    for (const auto& emp : employees) {
        double employeeTotalSalary = emp.basicSalary + emp.positionSalary + emp.allowance + emp.livingAllowance + emp.transportAllowance;
        cout << "======= ְ�����ܹ��ʱ� =======\n";
        cout << setw(6) << "����" << setw(12) << "�ܹ���" << setw(12) <<  endl;
        cout << setw(6) << emp.id << setw(12) << employeeTotalSalary << setw(12)  << endl;
        
        totalSalary += employeeTotalSalary;
        count++;
    }

    if (count > 0) {
        return totalSalary / count;
    }
    else {
        return 0.0;
    }
}

void querySalaryRank() {
    int empId;
    cout << "�����빤��: ";
    cin >> empId;

    sort(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) {
            return (a.basicSalary + a.positionSalary + a.allowance +
                a.livingAllowance + a.transportAllowance) >
                (b.basicSalary + b.positionSalary + b.allowance +
                    b.livingAllowance + b.transportAllowance);
        });

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });
    if (it != employees.end()) {
        int rank = distance(employees.begin(), it) + 1;
        cout << "�ܹ�������: " << rank << endl;
       
    }
    else {
        cout << "�Ҳ�������Ϊ " << empId << " ��ְ��\n";
    }
        calculateTotalRank();

}

void modifySalary() {
    int empId;
    cout << "�����빤��: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    if (it != employees.end()) {
        double oldSalary, newSalary;
        cout << "�����빤����Ŀ���ƣ���������ʡ�ְ���ʵȣ�: ";
        string salaryType;
        cin.ignore();
        getline(cin, salaryType);

        if (salaryType == "��������") {
            oldSalary = it->basicSalary;
            cout << "�������µĻ�������: ";
            cin >> newSalary;
            it->basicSalary = newSalary;
        }
        else if (salaryType == "ְ����") {
            oldSalary = it->positionSalary;
            cout << "�������µ�ְ����: ";
            cin >> newSalary;
            it->positionSalary = newSalary;
        }
        else if (salaryType == "����") {
            oldSalary = it->allowance;
            cout << "�������µĽ���: ";
            cin >> newSalary;
            it->allowance = newSalary;
        }
        else if (salaryType == "�����") {
            oldSalary = it->livingAllowance;
            cout << "�������µ������: ";
            cin >> newSalary;
            it->livingAllowance = newSalary;
        }
        else if (salaryType == "��ͨ����") {
            oldSalary = it->transportAllowance;
            cout << "�������µĽ�ͨ����: ";
            cin >> newSalary;
            it->transportAllowance = newSalary;
        }
        else {
            cout << "��Ч�Ĺ�����Ŀ\n";
            return;
        }

        cout << "�޸�ǰ���: " << fixed << setprecision(2) << oldSalary << endl;
        cout << "�޸ĺ���: " << fixed << setprecision(2) << newSalary << endl;
    }
    else {
        cout << "�Ҳ�������Ϊ " << empId << " ��ְ��\n";
    }
}

int main() {
    // ��ʼ��Ա����Ϣ
    employees = {
        {2301, 1235, 798, 1980, 260, 200},
        {2302, 1165, 648, 1350, 250, 200},
        {2303, 885, 442, 980, 200, 150},
        {2304, 1375, 875, 2100, 280, 200},
        {2305, 773, 396, 840, 200, 150},
        {2306, 1525, 910, 2050, 300, 200}
    };

    char choice;
    do {
        displayMenu();
        cout << "������ѡ�� (A/B/C/D/E)�����������ַ����˳�: ";
        cin >> choice;

        switch (choice) {
        case 'A':
            querySingleSalary();
            break;
        case 'B':
            queryTotalSalary();
            break;
        case 'C':
            querySalaryRank();
            break;
        case 'D':
            modifySalary();
            break;
        case 'E':
            PrintSalary();
            break;
        default:
            cout << "ллʹ��ְ�����ʹ���ϵͳ���ټ���\n";
            return 0;
        }

        // �ȴ�3��
        cout << "�������˵���...";
   
        
        Sleep(3000);
        cout << "\n\n";
    } while (true);

    return 0;
}

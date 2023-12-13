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
    cout << "======= 职工工资信息表 =======\n";
    cout << setw(6) << "工号" << setw(12) << "基本工资" << setw(12) << "职务工资"
        << setw(12) << "津贴" << setw(15) << "生活补贴" << setw(15) << "交通补贴" << endl;

    for (const auto& emp : employees) {
        cout << setw(6) << emp.id << setw(12) << emp.basicSalary << setw(12) << emp.positionSalary
            << setw(12) << emp.allowance << setw(15) << emp.livingAllowance << setw(15) << emp.transportAllowance << endl;
    }
}
void displayMenu() {
    cout << "======= 工资管理系统 =======\n";
    cout << "A. 查询单项工资\n";
    cout << "B. 查询总工资\n";
    cout << "C. 查询工资排名\n";
    cout << "D. 修改工资\n";
    cout << "E. 查看职工工资信息表\n";
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
        if (salaryType == "基本工资") {
            totalSalary += emp.basicSalary;
            count++;
        }
        else if (salaryType == "职务工资") {
            totalSalary += emp.positionSalary;
            count++;
        }
        else if (salaryType == "津贴") {
            totalSalary += emp.allowance;
            count++;
        }
        else if (salaryType == "生活补贴") {
            totalSalary += emp.livingAllowance;
            count++;
        }
        else if (salaryType == "交通补贴") {
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
    cout << "请输入工号: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    if (it != employees.end()) {
        double salary;
        cout << "请输入工资项目名称（如基本工资、职务工资等）: ";
        string salaryType;
        cin.ignore();
        getline(cin, salaryType);

        if (salaryType == "基本工资") {
            salary = it->basicSalary;
            salaryType = "基本工资";
        }
        else if (salaryType == "职务工资") {
            salary = it->positionSalary;
            salaryType = "职务工资";
        }
        else if (salaryType == "津贴") {
            salary = it->allowance;
            salaryType = "津贴";
        }
        else if (salaryType == "生活补贴") {
            salary = it->livingAllowance;
            salaryType = "生活补贴";
        }
        else if (salaryType == "交通补贴") {
            salary = it->transportAllowance;
            salaryType = "交通补贴";
        }
        else {
            cout << "无效的工资项目\n";
            return;
        }

        cout << "工资金额: " << fixed << setprecision(2) << salary << endl;
        double averageSalary = calculateAverage(salaryType);
        if (averageSalary > 0.0) {
            std::cout << "工资项目 " << salaryType << " 的平均工资为: " << std::fixed << std::setprecision(2) << averageSalary << std::endl;
        }
        else {
            std::cout << "无效的工资项目" << std::endl;
        }

        //计算该项工资的所有职工的平均工资
    }
    else {
        cout << "找不到工号为 " << empId << " 的职工\n";
    }
}

void queryTotalSalary() {
    int empId;
    cout << "请输入工号: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    
        double totalAverageSalary = calculateTotalAverage();
        if (totalAverageSalary > 0.0) {
            std::cout << "所有职工的平均总工资为: " << std::fixed << std::setprecision(2) << totalAverageSalary << std::endl;
        }
        else {
            std::cout << "无效的职工数据" << std::endl;
        }
   
}
double calculateTotalRank() {

    double totalSalary = 0.0;
    int count = 0;

    for (const auto& emp : employees) {
        double employeeTotalSalary = emp.basicSalary + emp.positionSalary + emp.allowance + emp.livingAllowance + emp.transportAllowance;
        cout << "======= 职工的总工资表 =======\n";
        cout << setw(6) << "工号" << setw(12) << "总工资" << setw(12) <<  endl;
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
    cout << "请输入工号: ";
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
        cout << "总工资排名: " << rank << endl;
       
    }
    else {
        cout << "找不到工号为 " << empId << " 的职工\n";
    }
        calculateTotalRank();

}

void modifySalary() {
    int empId;
    cout << "请输入工号: ";
    cin >> empId;

    auto it = find_if(employees.begin(), employees.end(),
        [empId](const Employee& emp) { return emp.id == empId; });

    if (it != employees.end()) {
        double oldSalary, newSalary;
        cout << "请输入工资项目名称（如基本工资、职务工资等）: ";
        string salaryType;
        cin.ignore();
        getline(cin, salaryType);

        if (salaryType == "基本工资") {
            oldSalary = it->basicSalary;
            cout << "请输入新的基本工资: ";
            cin >> newSalary;
            it->basicSalary = newSalary;
        }
        else if (salaryType == "职务工资") {
            oldSalary = it->positionSalary;
            cout << "请输入新的职务工资: ";
            cin >> newSalary;
            it->positionSalary = newSalary;
        }
        else if (salaryType == "津贴") {
            oldSalary = it->allowance;
            cout << "请输入新的津贴: ";
            cin >> newSalary;
            it->allowance = newSalary;
        }
        else if (salaryType == "生活补贴") {
            oldSalary = it->livingAllowance;
            cout << "请输入新的生活补贴: ";
            cin >> newSalary;
            it->livingAllowance = newSalary;
        }
        else if (salaryType == "交通补贴") {
            oldSalary = it->transportAllowance;
            cout << "请输入新的交通补贴: ";
            cin >> newSalary;
            it->transportAllowance = newSalary;
        }
        else {
            cout << "无效的工资项目\n";
            return;
        }

        cout << "修改前金额: " << fixed << setprecision(2) << oldSalary << endl;
        cout << "修改后金额: " << fixed << setprecision(2) << newSalary << endl;
    }
    else {
        cout << "找不到工号为 " << empId << " 的职工\n";
    }
}

int main() {
    // 初始化员工信息
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
        cout << "请输入选项 (A/B/C/D/E)，输入其他字符则退出: ";
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
            cout << "谢谢使用职工工资管理系统，再见！\n";
            return 0;
        }

        // 等待3秒
        cout << "返回主菜单中...";
   
        
        Sleep(3000);
        cout << "\n\n";
    } while (true);

    return 0;
}

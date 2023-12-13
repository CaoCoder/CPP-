
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
    printf("*****1. 根据工号查询单项工资 ******\n");
    printf("*****2. 根据工号查询总工资   ******\n");
    printf("*****3. 根据工号查询工资排名 ******\n");
    printf("*****4. 根据工号,工资修改工资******\n");
    printf("***********************************\n");
    printf("***********************************\n");
}


void modifySalary() {
    int empId;
    printf("请在这里输入工号！: ");
    cin >> empId;

    size_t i = 0;
    while (i < emps.size() && emps[i].id != empId) {
        ++i;
    }

    if (i < emps.size()) {
        double oldSalary, newSalary;
        printf("请输入工资项目名称: ");
        string st;
        cin.ignore();
        getline(cin, st);

        if (st == "基本工资") {
            oldSalary = emps[i].bs;
            printf("请输入新的基本工资: ");
            cin >> newSalary;
            emps[i].bs = newSalary;
        }
        else if (st == "职务工资") {
            oldSalary = emps[i].ps;
            printf("请输入新的职务工资: ");
            cin >> newSalary;
            emps[i].ps = newSalary;
        }
        else if (st == "津贴") {
            oldSalary = emps[i].al;
            printf("请输入新的津贴: ");
            cin >> newSalary;
            emps[i].al = newSalary;
        }
        else if (st == "生活补贴") {
            oldSalary = emps[i].la;
            printf("请输入新的生活补贴: ");
            cin >> newSalary;
            emps[i].la = newSalary;
        }
        else if (st == "交通补贴") {
            oldSalary = emps[i].ta;
            printf("请输入新的交通补贴: ");
            cin >> newSalary;
            emps[i].ta = newSalary;
        }
        else {
            printf("----------注意工资项目找不到\n");
            return;
        }

        printf("----------查看修改前金额: %.2f\n\n", oldSalary);
        printf("----------查看修改后金额: %.2f\n\n", newSalary);
    }
    else {
        printf("未找到工号为 %d 的职工\n", empId);
    }
}
// 计算所有员工的平均工资
double calcTotalAvg() {
    double totalSalary = 0.0; 
    int count = 0;  
    size_t i = 0;  

    // 遍历所有员工
    while (i < emps.size()) {
        
        totalSalary += emps[i].bs + emps[i].ps + emps[i].al + emps[i].la + emps[i].ta;
        count++;  
        i++;  
    }

    // 如果有员工，则返回平均工资否则返回0
    return (count > 0) ? totalSalary / count : 0.0;
}


double calcAvg(const std::string& type) {
    std::map<std::string, double Emp::* > typeMap = {
        { "基本工资", &Emp::bs },
        { "职务工资", &Emp::ps },
        { "津贴", &Emp::al },
        { "生活补贴", &Emp::la },
        { "交通补贴", &Emp::ta }
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
    printf("请在这里输入工号！: ");
    cin >> empId;

    size_t i = 0;
    while (i < emps.size() && emps[i].id != empId) {
        ++i;
    }

    if (i < emps.size()) {
        double salary;
        printf("请在这里输入工资项目的名称: ");
        string st;
        cin.ignore();
        getline(cin, st);

        if (st == "基本工资") {
            salary = emps[i].bs;
            st = "基本工资";
        }
        else if (st == "职务工资") {
            salary = emps[i].ps;
            st = "职务工资";
        }
        else if (st == "津贴") {
            salary = emps[i].al;
            st = "津贴";
        }
        else if (st == "生活补贴") {
            salary = emps[i].la;
            st = "生活补贴";
        }
        else if (st == "交通补贴") {
            salary = emps[i].ta;
            st = "交通补贴";
        }
        else {
            printf("找不到工资项目\n");
            return;
        }

        printf("工资金额: %.2f\n", salary);
        double averageSalary = calcAvg(st);
        if (averageSalary > 0.0) {
            printf("工资项目 %s 的平均工资为: %.2f\n", st.c_str(), averageSalary);
        }
        else {
            printf("找不到工资项目\n");
        }
    }
    else {
        printf("----------未找到工号为 %d 的职工\n", empId);
    }
}

void queryts() {
    int empId;
    std::cout << "请在这里输入工号！: ";
    std::cin >> empId;

    auto it = std::find_if(emps.begin(), emps.end(), [empId](const auto& emp) {
        return emp.id == empId;
        });

    double totalAverageSalary = calcTotalAvg();
    if (totalAverageSalary > 0.0) {
        std::cout << "输出所有职工的平均总工资为: " << std::fixed << std::setprecision(2) << totalAverageSalary << std::endl;
    }
    else {
        std::cout << "----------职工数据错误" << std::endl;
    }
}

double calcTotalRank() 
{
    double ts = 0.0;
    int count = 0;

    printf("***********请查看工资表**********\n");
    printf("%-6s%-12s%-12s\n", "工号", "总工资", "");

    for (size_t i = 0; i < emps.size(); ++i) {
        double employeets = emps[i].bs + emps[i].ps + emps[i].al + emps[i].la + emps[i].ta;
        printf("%-6d%-12.2f%-12s\n", emps[i].id, employeets, "");
        ts += employeets;
        count++;
    }

    return (count > 0) ? ts / count : 0.0;
}

// 查询工资排名
void querySalaryRank()
{
    int empId;  // 员工工号
    printf("请在这里输入工号！: ");
    cin >> empId;

    // 根据工资总和排序员工
    sort(emps.begin(), emps.end(),
        [](const Emp& a, const Emp& b) {
            return (a.bs + a.ps + a.al + a.la + a.ta) >
                (b.bs + b.ps + b.al + b.la + b.ta);
        });

    size_t i = 0;
    // 在排序后的员工列表中查找指定工号的员工
    while (i < emps.size() && emps[i].id != empId)
    {
        ++i;
    }

    if (i < emps.size())
    {
        // 计算员工的总工资排名
        int rank = distance(emps.begin(), emps.begin() + i) + 1;
        printf("总工资排名: %d\n", rank);
    }
    else
    {
        printf("找不到工号为 %d 的职工\n", empId);
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
        printf("请输入数字(1-4): ");
        cin >> n;

        switch (n) {
        case 1:
            printf("------1. 根据工号查询单项工资 ------\n");
          
            querySingleSalary();
            break;
         
        case 2:
            printf("------2. 根据工号查询总工资   ------\n");
        
            queryts();
            break;
        case 3:
            printf("------3. 根据工号查询工资排名 ------\n");
          
            querySalaryRank();
            break;
        case 4:
            printf("------4. 根据工号,工资修改工资------\n");
            modifySalary();
            break;
   
        default:
            printf("------输入错误，输入范围在1到4------\n");
            return 0;
        }

        printf("---------------------请注意，请注意,请注意！ 等待三秒后返回主菜单！-------------------");
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        system("cls");

        printf("\n\n");
    } while (1);

    return 0;
}

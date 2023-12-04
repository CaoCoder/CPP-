// test.cpp

#include "intern1.h"

// 实际的数据存储
vector<MonthlyInfo> monthlyInfos;

void menu()//菜单
{

    printf("**********************************\n");
    printf("**********************************\n");
    printf("********0.退出系统      **********\n");
    printf("********1.添加实习生信息**********\n");
    printf("********2.修改实习生信息**********\n");
    printf("********3.查询实习生信息**********\n");
    printf("********4.删除实习生信息**********\n");
    printf("********5.统计打印全部职工平均工资\n");
    printf("********6.打印实习生信息**********\n");
    printf("********7.保存数据到文件**********\n");
    printf("********8.读取存档      **********\n");
    printf("********9.根据月份对所有实习生的工资金额进行排序显示\n");
    printf("********10.按照ID号或者实习生姓名查询信息并统计平均工资\n");
    printf("********11.统计某个学校的实习生在某月份或者某年度的平均工资 \n");
    printf("请输入(0-10)：");

}

void menu1()//菜单1
{
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
    printf("----初次进入系统，可选择8先加载存档----\n");
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
  

}
void menu();

int main() {
   
    int n = 0;
   
    // 实际的数据存储
    vector<MonthlyInfo> monthlyInfos;
    menu1();
    do {
        menu();
        cin >> n;

        switch (n) {
        case 0:
            Sleep(600);

            cout << "感谢使用实习生工资管理系统，再见！\n";
            break;
        case 1:
            Sleep(600);

            system("cls");

            cout << "添加实习生信息\n";
            addIntern();

            break;
        case 2:
           

            Sleep(600);

            system("cls");

            cout << "修改实习生信息\n";
            modifyIntern();
            break;
        case 3:
            Sleep(600);

            system("cls");

            cout << "查询实习生信息\n";
            searchIntern();
            break;
        case 4:
            Sleep(600);

            system("cls");

            cout << "删除实习生信息\n";
            deleteIntern();
            break;
        case 5:
            Sleep(600);

            system("cls");

            cout << "统计平均工资 \n";
            sortByMonth();
            calculateAverageSalaryForAll();
            break;
        case 6:
            Sleep(600);
            system("cls");
            
            cout << "打印实习生信息\n";
            printAllInterns();
            
            break;
        case 7:
            Sleep(600);

            system("cls");

            cout << "保存数据到文件\n";
            saveDataToFile();
            break;
        case 8:
            Sleep(600);

            system("cls");

            cout << "加载读取数据文件\n";
            loadDataFromFile();
            break;
        case 9:
            Sleep(600);

            system("cls");

            cout << "按照月份排序\n";
            sortByMonth();
            break;
        case 10:
            Sleep(600);

            system("cls");

            cout << "统计\n";
            searchInternByIdOrName();
            break;
        case 11:
            Sleep(600);

            system("cls");

            cout << "统计\n";
            calculateAverageSalaryBySchool();
            break;
        default:
            system("cls");

            cout << "输入格式错误，请重新输入(0-10)\n";
        }

    } while (n);

    return 0;
}




// 添加实习生
void addIntern() {
    MonthlyInfo mn;
    cout << "请输入要添加实习生的姓名: ";
    cin >> mn.intern.name;
    cout << "请输入要添加实习生的ID号: ";
    cin >> mn.intern.id;
    cout << "请输入要添加实习生的学校: ";
    cin >> mn.intern.school;
    cout << "请输入要添加实习生的基本工资: ";
    cin >> mn.intern.baseSalary;
    cout << "请输入要添加实习生的职务工资: ";
    cin >> mn.intern.positionSalary;
    cout << "请输入要添加实习生的岗位津贴: ";
    cin >> mn.intern.allowance;
    cout << "请输入要添加实习生的医疗保险: ";
    cin >> mn.intern.medicalInsurance;
    cout << "请输入要添加实习生的实习天数: ";
    cin >> mn.intern.internshipDays;


    cout << "请输入要添加年份: ";
    cin >> mn.year;
    cout << "请输入要添加月份: ";
    cin >> mn.month;
  
    monthlyInfos.push_back(mn);
    cout << "添加实习生成功\n";
}

// 修改实习生信息
void modifyIntern() {
    if (monthlyInfos.empty()) {
        cout << "没有实习生信息可供修改\n";
        return;
    }

    int targetID;
    cout << "请输入要修改的实习生的ID号: ";
    cin >> targetID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [targetID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == targetID;
        });

    if (it != monthlyInfos.end()) {
        cout << "找到了要修改的实习生信息，请输入新的信息:\n";
        cout << "姓名: ";
        cin >> it->intern.name;
        cout << "学校: ";
        cin >> it->intern.school;
        cout << "基本工资: ";
        cin >> it->intern.baseSalary;
        cout << "职务工资: ";
        cin >> it->intern.positionSalary;
        cout << "岗位津贴: ";
        cin >> it->intern.allowance;
        cout << "医疗保险: ";
        cin >> it->intern.medicalInsurance;
        cout << "实习天数: ";
        cin >> it->intern.internshipDays;


        cout << "年: ";
        cin >> it->year;
        cout << "月: ";
        cin >> it->month;
        cout << "修改实习生信息成功\n";
    }
    else {
        cout << "未找到该ID的实习生信息\n";
    }
}

// 查询实习生信息
void searchIntern() {
    if (monthlyInfos.empty()) {
        cout << "没有实习生信息可供查询\n";
        return;
    }

    int searchID;
    cout << "请输入要查询的实习生的ID号: ";
    cin >> searchID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [searchID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == searchID;
        });

    if (it != monthlyInfos.end()) {
        cout << "找到了实习生信息:\n";
        cout << "姓名: " << it->intern.name << endl;
        cout << "学校: " << it->intern.school << endl;
        cout << "基本工资: " << it->intern.baseSalary << endl;
        cout << "职务工资: " << it->intern.positionSalary << endl;
        cout << "岗位津贴: " << it->intern.allowance << endl;
        cout << "医疗保险: " << it->intern.medicalInsurance << endl;
        cout << "实习天数: " << it->intern.internshipDays << endl;
        cout << "年: " << it->year<< endl;
        cout << "月: " << it->month << endl;
    }
    else {
        cout << "未找到该ID的实习生信息\n";
    }
}

// 删除实习生信息
void deleteIntern() {
    if (monthlyInfos.empty()) {
        cout << "没有实习生信息可供删除\n";
        return;
    }

    int deleteID;
    cout << "请输入要删除的实习生的ID号: ";
    cin >> deleteID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [deleteID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == deleteID;
        });

    if (it != monthlyInfos.end()) {
        monthlyInfos.erase(it);
        cout << "删除实习生信息成功\n";
    }
    else {
        cout << "未找到该ID的实习生信息\n";
    }
}


// 统计平均工资
void calculateAverageSalary() {
    if (monthlyInfos.empty()) {
        cout << "没有实习生信息可供统计\n";
        return;
    }

    double totalSalary = 0;
    for (const auto& monthlyInfo : monthlyInfos) {
        totalSalary += monthlyInfo.intern.baseSalary + monthlyInfo.intern.positionSalary
            + monthlyInfo.intern.allowance + monthlyInfo.intern.medicalInsurance;
    }

    double averageSalary = totalSalary / monthlyInfos.size();
    cout << "所有实习生的平均工资为: " << averageSalary << endl;
}


// 打印所有实习生信息
void printAllInterns() {
    printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",
        "编号", "姓名", "学校", "基本工资", "职务工资", "岗位津贴", "医疗保险", "实习天数","年", "月");

    for (const auto& monthlyInfo : monthlyInfos) {
        printf("%-10d\t%-10s\t%-10s\t%-10.2f\t%-10.2f\t%-10.2f\t%-10.2f\t%-10d\t%-10d\t%-10d\n",
            monthlyInfo.intern.id, monthlyInfo.intern.name.c_str(), monthlyInfo.intern.school.c_str(),
            monthlyInfo.intern.baseSalary, monthlyInfo.intern.positionSalary, monthlyInfo.intern.allowance,
            monthlyInfo.intern.medicalInsurance, monthlyInfo.intern.internshipDays, monthlyInfo.year, monthlyInfo.month);
    }
}

// 保存数据到文件
void saveDataToFile() {
    if (monthlyInfos.empty()) {
        cout << "没有实习生信息可供保存\n";
        return;
    }

    ofstream outputFile("interns_data.txt");

    if (!outputFile.is_open()) {
        cout << "无法打开文件来保存数据\n";
        return;
    }

    for (const auto& monthlyInfo : monthlyInfos) {
        outputFile << monthlyInfo.intern.id << "," << monthlyInfo.intern.name << "," << monthlyInfo.intern.school << ","
            << monthlyInfo.intern.baseSalary << "," << monthlyInfo.intern.positionSalary << ","
            << monthlyInfo.intern.allowance << "," << monthlyInfo.intern.medicalInsurance << ","
            << monthlyInfo.intern.internshipDays  <<","
            << monthlyInfo.year << "," << monthlyInfo.month << "\n";
    }

    outputFile.close();
    cout << "数据保存成功\n";
}
// 从文件读取数据
void loadDataFromFile() {
    ifstream inputFile("interns_data.txt");

    if (!inputFile.is_open()) {
        cout << "无法打开文件来读取数据\n";
        return;
    }

    monthlyInfos.clear();  // 清空已有数据

    while (inputFile) {
        MonthlyInfo mn;
        char comma;  

        inputFile >> mn.intern.id >> comma;
        if (inputFile.eof()) {
            break;  
        }

        getline(inputFile, mn.intern.name, ',');
        getline(inputFile, mn.intern.school, ',');
        inputFile >> mn.intern.baseSalary >> comma >> mn.intern.positionSalary
            >> comma >> mn.intern.allowance >> comma >> mn.intern.medicalInsurance
            >> comma >> mn.intern.internshipDays
            >> comma >> mn.year>> comma >> mn.month;

        monthlyInfos.push_back(mn);
    }

    inputFile.close();
    cout << "数据读取成功\n";
}

// 根据ID号或者实习生姓名查询信息并统计平均工资
void searchInternByIdOrName() {
    string searchName;
    int searchId;
    cout << "请输入要查询的实习生姓名或ID号: ";
    cin >> searchName;

    cout << "请输入要查询的实习生ID号（如果没有，请输入-1）: ";
    cin >> searchId;

    double totalSalary = 0;
    int totalCount = 0;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // 根据ID号或者姓名匹配实习生
        if ((searchId != -1 && intern.id == searchId) || (searchId == -1 && intern.name == searchName)) {
            // 打印实习生每个月的信息
            printf("实习生姓名：%s\n", intern.name.c_str());
            printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",
                "编号", "姓名", "学校", "基本工资", "职务工资", "岗位津贴", "医疗保险", "实习天数", "年", "月");

            // 打印月度信息
            printf("%-10d\t%-10s\t%-10s\t%-10.2f\t%-10.2f\t%-10.2f\t%-10.2f\t%-10d\t%-10d\t%-10d\n",
                intern.id, intern.name.c_str(), intern.school.c_str(),
                intern.baseSalary, intern.positionSalary, intern.allowance,
                intern.medicalInsurance, intern.internshipDays, monthlyInfo.year, monthlyInfo.month);

            // 统计平均工资
            totalSalary += (intern.baseSalary + intern.positionSalary + intern.allowance + intern.medicalInsurance);
            totalCount++;
        }
    }

    if (totalCount > 0) {
        double averageSalary = totalSalary / totalCount;
        printf("平均工资：%f\n", averageSalary);
    }
    else {
        cout << "未找到匹配的实习生信息\n";
    }
}

// 统计某个学校的实习生在某月份或者某年度的平均工资
void calculateAverageSalaryBySchool() {
    string searchSchool;
    int searchMonth;
    int searchYear;

    cout << "请输入要查询的学校名称: ";
    cin >> searchSchool;

    cout << "请输入要查询的月份（如果没有，请输入-1）: ";
    cin >> searchMonth;

    cout << "请输入要查询的年份（如果没有，请输入-1）: ";
    cin >> searchYear;

    double totalSalary = 0;
    int totalCount = 0;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // 根据学校匹配实习生
        if (intern.school == searchSchool &&
            (searchMonth == -1 || monthlyInfo.month == searchMonth) &&
            (searchYear == -1 || monthlyInfo.year == searchYear)) {
            // 统计平均工资
            totalSalary += (intern.baseSalary + intern.positionSalary + intern.allowance + intern.medicalInsurance);
            totalCount++;
        }
    }

    if (totalCount > 0) {
        double averageSalary = totalSalary / totalCount;
        printf("平均工资：%f\n", averageSalary);
    }
    else {
        cout << "未找到匹配的实习生信息\n";
    }
}



// 按月份对所有实习生的工资金额进行排序
void sortByMonth() {
    sort(monthlyInfos.begin(), monthlyInfos.end(),
        [](const MonthlyInfo& a, const MonthlyInfo& b) {
            if (a.year == b.year) {
                return a.month < b.month;
            }
            return a.year < b.year;
        });

    printAllInterns();

    cout << "按照月份排序成功" << endl;
}

// 计算所有实习生的平均工资
void calculateAverageSalaryForAll() {
    double totalSalary = 0;
    int totalCount = 0;

    unordered_set<int> uniqueIds;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // 使用unordered_set来检查ID是否已经存在，进行去重
        if (uniqueIds.find(intern.id) == uniqueIds.end()) {
           
            totalCount++;
            uniqueIds.insert(intern.id);
        }
    }
 
    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;
        totalSalary += (intern.baseSalary + intern.positionSalary +
            intern.allowance + intern.medicalInsurance);
       
    }

    if (totalCount > 0) {
        double averageSalary = totalSalary / totalCount;
        cout << "所有实习生的平均工资：" << averageSalary << endl;
    }
    else {
        cout << "暂无实习生信息\n";
    }
}


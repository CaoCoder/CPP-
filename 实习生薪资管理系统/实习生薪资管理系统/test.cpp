// test.cpp

#include "intern1.h"

// ʵ�ʵ����ݴ洢
vector<MonthlyInfo> monthlyInfos;

void menu()//�˵�
{

    printf("**********************************\n");
    printf("**********************************\n");
    printf("********0.�˳�ϵͳ      **********\n");
    printf("********1.���ʵϰ����Ϣ**********\n");
    printf("********2.�޸�ʵϰ����Ϣ**********\n");
    printf("********3.��ѯʵϰ����Ϣ**********\n");
    printf("********4.ɾ��ʵϰ����Ϣ**********\n");
    printf("********5.ͳ�ƴ�ӡȫ��ְ��ƽ������\n");
    printf("********6.��ӡʵϰ����Ϣ**********\n");
    printf("********7.�������ݵ��ļ�**********\n");
    printf("********8.��ȡ�浵      **********\n");
    printf("********9.�����·ݶ�����ʵϰ���Ĺ��ʽ�����������ʾ\n");
    printf("********10.����ID�Ż���ʵϰ��������ѯ��Ϣ��ͳ��ƽ������\n");
    printf("********11.ͳ��ĳ��ѧУ��ʵϰ����ĳ�·ݻ���ĳ��ȵ�ƽ������ \n");
    printf("������(0-10)��");

}

void menu1()//�˵�1
{
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
    printf("----���ν���ϵͳ����ѡ��8�ȼ��ش浵----\n");
    printf("---------------------------------------\n");
    printf("---------------------------------------\n");
  

}
void menu();

int main() {
   
    int n = 0;
   
    // ʵ�ʵ����ݴ洢
    vector<MonthlyInfo> monthlyInfos;
    menu1();
    do {
        menu();
        cin >> n;

        switch (n) {
        case 0:
            Sleep(600);

            cout << "��лʹ��ʵϰ�����ʹ���ϵͳ���ټ���\n";
            break;
        case 1:
            Sleep(600);

            system("cls");

            cout << "���ʵϰ����Ϣ\n";
            addIntern();

            break;
        case 2:
           

            Sleep(600);

            system("cls");

            cout << "�޸�ʵϰ����Ϣ\n";
            modifyIntern();
            break;
        case 3:
            Sleep(600);

            system("cls");

            cout << "��ѯʵϰ����Ϣ\n";
            searchIntern();
            break;
        case 4:
            Sleep(600);

            system("cls");

            cout << "ɾ��ʵϰ����Ϣ\n";
            deleteIntern();
            break;
        case 5:
            Sleep(600);

            system("cls");

            cout << "ͳ��ƽ������ \n";
            sortByMonth();
            calculateAverageSalaryForAll();
            break;
        case 6:
            Sleep(600);
            system("cls");
            
            cout << "��ӡʵϰ����Ϣ\n";
            printAllInterns();
            
            break;
        case 7:
            Sleep(600);

            system("cls");

            cout << "�������ݵ��ļ�\n";
            saveDataToFile();
            break;
        case 8:
            Sleep(600);

            system("cls");

            cout << "���ض�ȡ�����ļ�\n";
            loadDataFromFile();
            break;
        case 9:
            Sleep(600);

            system("cls");

            cout << "�����·�����\n";
            sortByMonth();
            break;
        case 10:
            Sleep(600);

            system("cls");

            cout << "ͳ��\n";
            searchInternByIdOrName();
            break;
        case 11:
            Sleep(600);

            system("cls");

            cout << "ͳ��\n";
            calculateAverageSalaryBySchool();
            break;
        default:
            system("cls");

            cout << "�����ʽ��������������(0-10)\n";
        }

    } while (n);

    return 0;
}




// ���ʵϰ��
void addIntern() {
    MonthlyInfo mn;
    cout << "������Ҫ���ʵϰ��������: ";
    cin >> mn.intern.name;
    cout << "������Ҫ���ʵϰ����ID��: ";
    cin >> mn.intern.id;
    cout << "������Ҫ���ʵϰ����ѧУ: ";
    cin >> mn.intern.school;
    cout << "������Ҫ���ʵϰ���Ļ�������: ";
    cin >> mn.intern.baseSalary;
    cout << "������Ҫ���ʵϰ����ְ����: ";
    cin >> mn.intern.positionSalary;
    cout << "������Ҫ���ʵϰ���ĸ�λ����: ";
    cin >> mn.intern.allowance;
    cout << "������Ҫ���ʵϰ����ҽ�Ʊ���: ";
    cin >> mn.intern.medicalInsurance;
    cout << "������Ҫ���ʵϰ����ʵϰ����: ";
    cin >> mn.intern.internshipDays;


    cout << "������Ҫ������: ";
    cin >> mn.year;
    cout << "������Ҫ����·�: ";
    cin >> mn.month;
  
    monthlyInfos.push_back(mn);
    cout << "���ʵϰ���ɹ�\n";
}

// �޸�ʵϰ����Ϣ
void modifyIntern() {
    if (monthlyInfos.empty()) {
        cout << "û��ʵϰ����Ϣ�ɹ��޸�\n";
        return;
    }

    int targetID;
    cout << "������Ҫ�޸ĵ�ʵϰ����ID��: ";
    cin >> targetID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [targetID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == targetID;
        });

    if (it != monthlyInfos.end()) {
        cout << "�ҵ���Ҫ�޸ĵ�ʵϰ����Ϣ���������µ���Ϣ:\n";
        cout << "����: ";
        cin >> it->intern.name;
        cout << "ѧУ: ";
        cin >> it->intern.school;
        cout << "��������: ";
        cin >> it->intern.baseSalary;
        cout << "ְ����: ";
        cin >> it->intern.positionSalary;
        cout << "��λ����: ";
        cin >> it->intern.allowance;
        cout << "ҽ�Ʊ���: ";
        cin >> it->intern.medicalInsurance;
        cout << "ʵϰ����: ";
        cin >> it->intern.internshipDays;


        cout << "��: ";
        cin >> it->year;
        cout << "��: ";
        cin >> it->month;
        cout << "�޸�ʵϰ����Ϣ�ɹ�\n";
    }
    else {
        cout << "δ�ҵ���ID��ʵϰ����Ϣ\n";
    }
}

// ��ѯʵϰ����Ϣ
void searchIntern() {
    if (monthlyInfos.empty()) {
        cout << "û��ʵϰ����Ϣ�ɹ���ѯ\n";
        return;
    }

    int searchID;
    cout << "������Ҫ��ѯ��ʵϰ����ID��: ";
    cin >> searchID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [searchID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == searchID;
        });

    if (it != monthlyInfos.end()) {
        cout << "�ҵ���ʵϰ����Ϣ:\n";
        cout << "����: " << it->intern.name << endl;
        cout << "ѧУ: " << it->intern.school << endl;
        cout << "��������: " << it->intern.baseSalary << endl;
        cout << "ְ����: " << it->intern.positionSalary << endl;
        cout << "��λ����: " << it->intern.allowance << endl;
        cout << "ҽ�Ʊ���: " << it->intern.medicalInsurance << endl;
        cout << "ʵϰ����: " << it->intern.internshipDays << endl;
        cout << "��: " << it->year<< endl;
        cout << "��: " << it->month << endl;
    }
    else {
        cout << "δ�ҵ���ID��ʵϰ����Ϣ\n";
    }
}

// ɾ��ʵϰ����Ϣ
void deleteIntern() {
    if (monthlyInfos.empty()) {
        cout << "û��ʵϰ����Ϣ�ɹ�ɾ��\n";
        return;
    }

    int deleteID;
    cout << "������Ҫɾ����ʵϰ����ID��: ";
    cin >> deleteID;

    auto it = find_if(monthlyInfos.begin(), monthlyInfos.end(), [deleteID](const MonthlyInfo& monthlyInfo) {
        return monthlyInfo.intern.id == deleteID;
        });

    if (it != monthlyInfos.end()) {
        monthlyInfos.erase(it);
        cout << "ɾ��ʵϰ����Ϣ�ɹ�\n";
    }
    else {
        cout << "δ�ҵ���ID��ʵϰ����Ϣ\n";
    }
}


// ͳ��ƽ������
void calculateAverageSalary() {
    if (monthlyInfos.empty()) {
        cout << "û��ʵϰ����Ϣ�ɹ�ͳ��\n";
        return;
    }

    double totalSalary = 0;
    for (const auto& monthlyInfo : monthlyInfos) {
        totalSalary += monthlyInfo.intern.baseSalary + monthlyInfo.intern.positionSalary
            + monthlyInfo.intern.allowance + monthlyInfo.intern.medicalInsurance;
    }

    double averageSalary = totalSalary / monthlyInfos.size();
    cout << "����ʵϰ����ƽ������Ϊ: " << averageSalary << endl;
}


// ��ӡ����ʵϰ����Ϣ
void printAllInterns() {
    printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",
        "���", "����", "ѧУ", "��������", "ְ����", "��λ����", "ҽ�Ʊ���", "ʵϰ����","��", "��");

    for (const auto& monthlyInfo : monthlyInfos) {
        printf("%-10d\t%-10s\t%-10s\t%-10.2f\t%-10.2f\t%-10.2f\t%-10.2f\t%-10d\t%-10d\t%-10d\n",
            monthlyInfo.intern.id, monthlyInfo.intern.name.c_str(), monthlyInfo.intern.school.c_str(),
            monthlyInfo.intern.baseSalary, monthlyInfo.intern.positionSalary, monthlyInfo.intern.allowance,
            monthlyInfo.intern.medicalInsurance, monthlyInfo.intern.internshipDays, monthlyInfo.year, monthlyInfo.month);
    }
}

// �������ݵ��ļ�
void saveDataToFile() {
    if (monthlyInfos.empty()) {
        cout << "û��ʵϰ����Ϣ�ɹ�����\n";
        return;
    }

    ofstream outputFile("interns_data.txt");

    if (!outputFile.is_open()) {
        cout << "�޷����ļ�����������\n";
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
    cout << "���ݱ���ɹ�\n";
}
// ���ļ���ȡ����
void loadDataFromFile() {
    ifstream inputFile("interns_data.txt");

    if (!inputFile.is_open()) {
        cout << "�޷����ļ�����ȡ����\n";
        return;
    }

    monthlyInfos.clear();  // �����������

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
    cout << "���ݶ�ȡ�ɹ�\n";
}

// ����ID�Ż���ʵϰ��������ѯ��Ϣ��ͳ��ƽ������
void searchInternByIdOrName() {
    string searchName;
    int searchId;
    cout << "������Ҫ��ѯ��ʵϰ��������ID��: ";
    cin >> searchName;

    cout << "������Ҫ��ѯ��ʵϰ��ID�ţ����û�У�������-1��: ";
    cin >> searchId;

    double totalSalary = 0;
    int totalCount = 0;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // ����ID�Ż�������ƥ��ʵϰ��
        if ((searchId != -1 && intern.id == searchId) || (searchId == -1 && intern.name == searchName)) {
            // ��ӡʵϰ��ÿ���µ���Ϣ
            printf("ʵϰ��������%s\n", intern.name.c_str());
            printf("%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\t%-10s\n",
                "���", "����", "ѧУ", "��������", "ְ����", "��λ����", "ҽ�Ʊ���", "ʵϰ����", "��", "��");

            // ��ӡ�¶���Ϣ
            printf("%-10d\t%-10s\t%-10s\t%-10.2f\t%-10.2f\t%-10.2f\t%-10.2f\t%-10d\t%-10d\t%-10d\n",
                intern.id, intern.name.c_str(), intern.school.c_str(),
                intern.baseSalary, intern.positionSalary, intern.allowance,
                intern.medicalInsurance, intern.internshipDays, monthlyInfo.year, monthlyInfo.month);

            // ͳ��ƽ������
            totalSalary += (intern.baseSalary + intern.positionSalary + intern.allowance + intern.medicalInsurance);
            totalCount++;
        }
    }

    if (totalCount > 0) {
        double averageSalary = totalSalary / totalCount;
        printf("ƽ�����ʣ�%f\n", averageSalary);
    }
    else {
        cout << "δ�ҵ�ƥ���ʵϰ����Ϣ\n";
    }
}

// ͳ��ĳ��ѧУ��ʵϰ����ĳ�·ݻ���ĳ��ȵ�ƽ������
void calculateAverageSalaryBySchool() {
    string searchSchool;
    int searchMonth;
    int searchYear;

    cout << "������Ҫ��ѯ��ѧУ����: ";
    cin >> searchSchool;

    cout << "������Ҫ��ѯ���·ݣ����û�У�������-1��: ";
    cin >> searchMonth;

    cout << "������Ҫ��ѯ����ݣ����û�У�������-1��: ";
    cin >> searchYear;

    double totalSalary = 0;
    int totalCount = 0;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // ����ѧУƥ��ʵϰ��
        if (intern.school == searchSchool &&
            (searchMonth == -1 || monthlyInfo.month == searchMonth) &&
            (searchYear == -1 || monthlyInfo.year == searchYear)) {
            // ͳ��ƽ������
            totalSalary += (intern.baseSalary + intern.positionSalary + intern.allowance + intern.medicalInsurance);
            totalCount++;
        }
    }

    if (totalCount > 0) {
        double averageSalary = totalSalary / totalCount;
        printf("ƽ�����ʣ�%f\n", averageSalary);
    }
    else {
        cout << "δ�ҵ�ƥ���ʵϰ����Ϣ\n";
    }
}



// ���·ݶ�����ʵϰ���Ĺ��ʽ���������
void sortByMonth() {
    sort(monthlyInfos.begin(), monthlyInfos.end(),
        [](const MonthlyInfo& a, const MonthlyInfo& b) {
            if (a.year == b.year) {
                return a.month < b.month;
            }
            return a.year < b.year;
        });

    printAllInterns();

    cout << "�����·�����ɹ�" << endl;
}

// ��������ʵϰ����ƽ������
void calculateAverageSalaryForAll() {
    double totalSalary = 0;
    int totalCount = 0;

    unordered_set<int> uniqueIds;

    for (const auto& monthlyInfo : monthlyInfos) {
        const Intern& intern = monthlyInfo.intern;

        // ʹ��unordered_set�����ID�Ƿ��Ѿ����ڣ�����ȥ��
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
        cout << "����ʵϰ����ƽ�����ʣ�" << averageSalary << endl;
    }
    else {
        cout << "����ʵϰ����Ϣ\n";
    }
}


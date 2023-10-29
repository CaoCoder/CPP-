// #include <iostream>
// #include <cmath>

// class Z {
// public:
//     bool check(int q[], int j) { // 检查第j个皇后是否合法
//         for (int i = 1; i < j; i++) {
//             if (q[i] == q[j] || std::abs(q[i] - q[j]) == std::abs(i - j)) {
//                 return true;
//             }
//         }
//         return false;
//     }
// };

// int main() {
//     int n = 4;
//     int q[n + 1]; // 皇后位置
//     int i = 1;
//     for (; i <= n; i++) {
//         q[i] = 0;
//     }
//     int answer = 1;
//     int j = 1;
//     Z z;
//     while (j >= 1) {
//         q[j] = q[j] + 1; // 第j个皇后当前位置
//         while (q[j] <= n && z.check(q, j)) { // 判断第j个皇后是否合法
//             q[j] = q[j] + 1; // 不合法则位置加一
//         }
//         if (q[j] <= n) {
//             if (j == n) {
//                 std::cout << "方案" << answer++ << ":";
//                 for (; j <= n; j++) {
//                     std::cout << q[j] << "\t"; // 输出n皇后的一组解
//                 }
//                 std::cout << "\n";
//             } else {
//                 j = j + 1; // 继续往后排列
//             }
//         } else { // 第j个皇后找不到合法位置
//             q[j] = 0;
//             j = j - 1; // 回溯
//         }
//     }

//     return 0;
// }

#include <iostream>
// #include <cmath>
// #include <iostream>
// #include <cmath>

// class Z {
// public:
//     bool check(int q[], int j) { 
//         for (int i = 1; i < j; i++) {
//             if (q[i] == q[j] || std::abs(q[i] - q[j]) == std::abs(i - j)) {
//                 // 检查第j个皇后是否与之前的皇后冲突
//                 // 冲突条件：在同一列或同一斜线上
//                 return true;
//             }
//         }
//         return false;
//     }
// };

// int main() {
//     int n = 4;
//     int q[n + 1]; // 皇后位置
//     int i = 1;
//     for (; i <= n; i++) {
//         q[i] = 0;
//     }
//     int answer = 1; // 方案计数器
//     int j = 1; // 当前处理的皇后编号
//     Z z;
//     while (j >= 1) {
//         q[j] = q[j] + 1; // 第j个皇后当前位置
//         while (q[j] <= n && z.check(q, j)) { // 判断第j个皇后是否合法
//             q[j] = q[j] + 1; // 不合法则位置加一
//         }
//         if (q[j] <= n) {
//             if (j == n) {
//                 std::cout << "方案" << answer++ << ": ";
//                 for (int k = 1; k <= n; k++) {
//                     std::cout << q[k] << "\t"; // 输出n皇后的一组解
//                 }
//                 std::cout << "\n";
//             } else {
//                 j = j + 1; // 继续往后排列下一个皇后
//             }
//         } else { // 第j个皇后找不到合法位置
//             q[j] = 0; // 重置第j个皇后的位置
//             j = j - 1; // 回溯到前一个皇后
//         }
//     }

//     return 0;
// }

// #include <iostream>
// using namespace std;
// int main()
// {
//     auto b= (+'a');
//     auto c= 'A' ;
//     cout << b << endl;
//     cout << c;
//     return 0;
// }
#include <iostream>


int main()
{
    char ch = 'a';
    char temp = ch - 32;
    std::cout <<  temp << std::endl;
    return 0;
}
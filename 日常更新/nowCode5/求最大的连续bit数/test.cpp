// #include <iostream>
// #include <bitset>
// using namespace std;

// int main()
//  {
//     int n = 0;
//     cin >> n;
//     bitset<32> b(n);

   
//     int c = 0;
//     int temp = 0;
//     for(int i = b.size()-1; i >= 0 ; i--)
//     {
        
//         //  cout << b[i] << " ";
//         if(b[i] == 1)
//         {
//             c++;
//             while(i > 0 && b[i-1] == 1)
//             {
//                 c++;
//                 i--;
//             }
//         }
//         else
//         {
//             if(temp < c)
//             {
//                 temp = c;

//             }
//             c=0;
//         }
//         if(temp < c)
//         {
//             temp = c;
//         }

//     }
   
//     cout << temp;
    
// }
// 64 位输出请用 printf("%lld")

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>

int main() {
    std::set<int> generatedNumbers;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    int count = 0;
    while (count < 100) {
        int randomNumber = std::rand() % 900000 + 100000; // 生成6位数
        if (generatedNumbers.find(randomNumber) == generatedNumbers.end()) {
            generatedNumbers.insert(randomNumber);
            std::cout << randomNumber << std::endl;
            count++;
        }
    }
    
    return 0;
}

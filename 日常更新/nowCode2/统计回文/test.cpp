#include <iostream>
#include <string>
using namespace std;
#include <algorithm>
int main() 
{
   string s1;
   string s2;

   cin >> s1;
   cin >> s2;
   int count = 0;
   string s3 = s1;
   string s4;
   for(int i = 0; i <= s1.length(); i++)
   {
        s3 = s1;
        s3 = s3.insert(i, s2);
        s4 = s3;
        reverse(s4.begin(), s4.end());
        if(s3 == s4)
        {
            count++;
        }
   }
   cout<< count;
}
// 64 位输出请用 printf("%lld")
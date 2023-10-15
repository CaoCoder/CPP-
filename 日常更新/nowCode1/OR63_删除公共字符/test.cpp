#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s1;
    getline(cin, s1);
    string s2;
    cin >> s2;
    
    for(auto e : s2)
    {
        size_t n = s1.find(e);
      
        while(n != string::npos)
        {
            s1.erase(n, 1);
           
            n = s1.find(e);
        }
        //  cout << e;
    }
     cout << s1;
}
// 64 位输出请用 printf("%lld")
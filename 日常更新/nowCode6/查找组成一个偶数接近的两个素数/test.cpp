#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v;
    int n;
    cin >> n;
    int flags = 1;
    //输出所有素数
    for(int i = 2; i <= n ;i++)
    {
        flags = 1;
        for(int j = 2; j < i; j++)
        {
            if(i % j == 0)
            {
                flags = 0;
                break;
            }
        }
        if(flags)
        {
            v.push_back(i);
        }
    }
  
    int max = 0;
    int a1 = 0;
    int a2 = 0;
    for (int i = 0; i < v.size(); i++)
    {
        for(int j = i; j < v.size() ;j++)
        {
            if(v[i] + v[j] == n)
            {
                // cout << v[i] << " " << v[j];
                if(abs(v[i]-v[j]) >= max)
                {
                    a1 = v[i];
                    a2 = v[j];
                }
            }
        }
       
    }
   cout << a1 << endl;
   cout << a2 << endl;
   
}
// 64 位输出请用 printf("%lld")
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n = 0;
    cin >> n;
    vector<int> v;
    v.resize(2, 0);
    v[0] = 0;
    v[1] = 1;
    int a = 0,b = 0;
    for(int i = 2; i >= 2; i++)
    {
        v[i] = v[i-1] + v[i-2];

        if(n < v[i])
        {
            a = n - v[i-1];
            b = v[i] - n;
            break;
        }
   
    }
    if(a < b)
        cout << a;
    else
        cout << b;


   
}
// 64 位输出请用 printf("%lld")
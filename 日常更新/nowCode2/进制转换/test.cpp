#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() 
{
    int m = 0;
    int n = 0;
    cin >> m;
    cin >> n;
    string s1;
    int temp = 0;
    
    int t = m;
    m = abs(m);
    if(m > 0)
    {
        while(m)
        {
            temp = m % n;//得到每个位上的数
            if(temp >= 10)
            {
                if(temp == 10)
                {
                    s1 += "A";
                }
               
                 if(temp == 11)
                {
                    s1 += "B";
                }
                 if(temp == 12)
                {
                    s1 += "C";
                }
                 if(temp == 13)
                {
                    s1 += "D";
                }
                 if(temp == 14)
                {
                    s1 += "E";
                }
                 if(temp == 15)
                {
                    s1 += "F";
                }
            }
            else
            {
                s1 += to_string(temp);
            }
            // cout << s1 << endl;
            m = m / n;//迭代
        }
         reverse(s1.begin(),s1.end());
    }
   
    else
    {
        cout << 0;
    }
    if(t < 0)
    {
        s1.insert(0, "-");
    }

    
    cout << s1;
}
// 64 位输出请用 printf("%lld")
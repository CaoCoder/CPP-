#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() 
{
    vector<string> v;
    string s1;
    cin >> s1;
    
    string::iterator it  = s1.begin();
    string s2;
    while(it != s1.end())
    {
        if(*it >= '1' && *it <= '9')
        {
            
            //循环拿到连续字符串
            while(it != s1.end() && *it + 1 == *(it+1))
            {
                s2 += *it;
                ++it;
            }
            s2 += *it;
            //加入字符串
            v.push_back(s2);
            s2.clear();//清空字符串
            ++it;
        }
        else
        {
            ++it;
        }
    }
    string s3;
    int t = 0;
    for(auto e : v)
    {
        if(e.size() > t)
        {
            t = e.size();
            s3 = e;
        }
    }
    cout << s3;
}
// 64 位输出请用 printf("%lld")
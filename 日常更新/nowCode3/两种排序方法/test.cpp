#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;


bool myfunc(string& s1, string& s2)
{
    return s1.length() < s2.length();
}
bool myfunc1(string& s1, string& s2)
{
    // return strcmp(s1.c_str(), s2.c_str()) < 0;
    return s1.compare(s2) < 0;
}
int main()
{
   int n = 0;
   cin >> n;
   vector<string> v;
   v.resize(n, "");
   for(int i = 0; i < n; i++)
   {
        cin >> v[i];
   }

    vector<string> v1 = v;
    vector<string> v2 = v;
   //按长度比较
   sort(v1.begin(), v1.end(), myfunc);
    //按字符比较
   sort(v2.begin(), v2.end(), myfunc1);
    int count = 0;
    int count1 = 0;
    for(int i = 0; i < v.size(); i++)
    {
        if(v[i] != v1[i] && v[i]!= v2[i])
        {
            cout << "none";
            break;
        }
        if(v[i] == v1[i])
        {
            count++;
        }
        if(v[i] == v2[i])
        {
            count1++;
        }
        // else if(v[i] == v1[i] && v[i]== v2[i])
        // {
        //     cout << "both";
        // }
        // else if(v[i] == v1[i] && v[i] != v2[i])
        // {
        //     cout << "lexicographically";ss

        // }
        // else if(v[i] != v1[i] && v[i] == v2[i])
        // {
        //     cout << "lengths";

        // }
    }
    if(count == v.size() && count1 != v.size())
         cout << "lengths";
    else if(count1 == v.size() && count != v.size())
        cout << "lexicographically";
    else if(count == count1 && count == v.size())
        cout << "both";
  
}
// 64 位输出请用 printf("%lld")
class A
{
public:
    static A* createA()
    {
        return new A;
    }
private:
    A(){}
};

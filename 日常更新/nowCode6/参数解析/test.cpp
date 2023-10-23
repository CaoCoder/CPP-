#include <iostream>
#include <string>
#include <algorithm>
#include<vector>
using namespace std;

int main()
{
    vector<string> vs;
    int count = 0;
    string s2;
    string s1;
    getline(cin , s1);
    int start1 = 0;
    int end1 = 0;
    for(int i = 0; i < s1.size(); i++)
    {
       
        if(s1[i] == '"')
        {
            
            i++;
            start1 = i;
            while(i < s1.size() && s1[i] != '"')
            {
                i++;
            }
            end1 = i-start1;
            i++;
            s2 = s1.substr(start1, end1);
            // cout << s2 << endl;
            vs.push_back(s2);
            count++;
            s2.clear();
        }

        if(i < s1.size() && s1[i] != ' ')
        {
            while(i < s1.size() && s1[i] != ' ')
            {
                s2 += s1[i];
                i++;
            }
            // cout << s2 << endl;
            vs.push_back(s2);

            count++;

            s2.clear();
        }
    }

    cout << count << endl;
    for(auto& e: vs)
    {
        cout << e << endl;
    }

}
// 64 位输出请用 printf("%lld")
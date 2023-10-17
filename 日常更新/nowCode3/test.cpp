#include <iostream>
#include <string>
using namespace std;
class Solution 
{
public:
    int StrToInt(string str) 
    {
        
        int i = 0;
        string temp;
        char t = str[i];
        if((t < '1' || t > '9') && (t != '+' && t != '-'))
        {
            return 0;
        }
        for(i = 1; i < str.length(); i++)
        {


            if(str[i] < '1' || str[i] > '9')
            {
                return 0;
            }
            else 
            {
                temp += str[i];
            }
        }
       
        
       
        if(t == '+')
        {
            if(temp.size() == 0)
            {
                return 0;
            }
        
            else 
            {
                int n1 = atoi(string(temp.begin(), temp.end()).c_str());
                // cout << n1; 
                return n1;
                
            }
        }
        else if(t >= '1' || t <= '9')
        {
             return atoi((temp.insert(0,1 ,t)).c_str());
        }
        else
        {
            return atoi(temp.c_str());
        }
    }
};

int main()
{
    Solution s;
    string s1 = "+";
    cout << s.StrToInt(s1);
}
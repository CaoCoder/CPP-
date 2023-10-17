#include <iostream>
#include <string>
#include <stack>

using namespace std;
stack<char> sk;
class Parenthesis 
{
public:
    bool chkParenthesis(string A, int n) 
    {
        for(auto e : A)
        {
            // cout << e << " ";
            if(e == '(')
            {
                sk.push(e);
            }
            else if(e == ')')
            {
                if(!sk.empty())
                    sk.pop();
                else
                    return false;
            }
        }

        if(!sk.empty())
            return false;
        else
            return true;
    }
};

int main()
{
    Parenthesis p;
    string s = "())()()(((d";
    p.chkParenthesis(s, 11);
}

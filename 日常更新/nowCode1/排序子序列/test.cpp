#include <iostream>
#include <vector>

using namespace std;

int main() 
{
   
   int n = 0;
   cin >> n;
   int num = 0;
   vector<int> v;
   for(int i = 0; i < n; i++)
   {
        cin >> num;
        v.push_back(num);
   }
    int count = 0;
    int i = 0;
    int flags = 0;
    while(i < n)
    {
        //如果是递增序列
        if(v[i] < v[i+1])
        {
            
            flags = 0;//进入递增序列
            i++;
        }
        //假如相等后，判断是递增还是递减
        else if(v[i] == v[i+1] && flags == 0)
        {
            i++;
        }
        else if(flags == 1)
        {
            i++;
        }
        else
        {
            flags = 1;//开始进入递减序列
            count++;
        }
    }
    i = 0;
    flags = 0;
    while(i < n)
    {
        //如果是递增序列
        if(v[i] >= v[i+1])
        {
            flags = 0;//进入递减序列
            i++;
        }
         //假如相等后，判断是递增还是递减
        else if(v[i] == v[i+1] && flags == 1)
        {
            i++;
        }
        else if(flags == 1)
        {
            i++;
        }
        else
        {
            flags = 1;//开始进入递增序列
            count++;
        }
    }
    cout << count;
}

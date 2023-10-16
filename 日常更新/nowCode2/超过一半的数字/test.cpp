#include <vector>
#include <unordered_map>
using namespace std;
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param numbers int整型vector 
     * @return int整型
     */
    int MoreThanHalfNum_Solution(vector<int>& numbers) 
    {
        
        unordered_map<int, int> map1;
        for(auto e : numbers)
        {
           if(!map1.count(e))
           {
                map1[e] = 1;
           }
           else 
           {
                map1[e]++;
           }
        }
        int t = 0;
        int value;
        for(auto e : map1)
        {
           if(e.second > t)
           {
                t = e.second;
                value = e.first;
           }
        }
        return value;
    }
    
};
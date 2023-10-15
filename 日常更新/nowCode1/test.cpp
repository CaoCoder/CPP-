#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() 
{
    vector<int> v;
    int n = 0;
    cin >> n;
    int num;
    for(int i = 0; i < 3*n; i++)
    {
        cin >> num;
        v.push_back(num);
    }
    sort(v.begin(), v.end());
    vector<int> subvector(v.begin()+n, v.end());
    
    int sum = 0;
    int count = 0;
    for(auto e : subvector)
    {
        if(count % 2 == 0)
        {
            sum += e;
        }
        count++;
    }
    cout <<sum << " ";
}
// 64 位输出请用 printf("%lld")
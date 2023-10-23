#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;
    vector<int> v1;
    vector<int> v2;
    int left = 0;
    int count = 0;
    int flags = 0;
    if ((m - n) % n == 0)
    {
        count = (m - n) / n;
        cout << count;
        return 1;
    }
    else
    {
        left = (m - n) % n;
        // cout << "比较" << endl;
        for (int i = 2; i <= n; i++)
        {
            if (n % i == 0)
            {
                // if (left == i)
                // {
                //     count++;
                //     flags = 1;
                // }
                v1.push_back(i);
                v2.push_back(i);
            }
        }
        for(int i = 0; i < v1.size(); i++)
        {
            for(int j = i + 1; j < v1.size(); j++)
            {
                v2.push_back(v1[i]+ v1[j]);
            }
        }
        for(auto e : v2)
        {
            cout << e << " ";
        }
        for (int i = 0; i < v2.size(); i++)
        {
            if (n % v2[i] == 0)
            {
                if (left == i)
                {
                    count++;
                    flags = 1;
                }
            }
        }
        if(flags)
        {
            cout << count;
        }
        else
        {
            cout << -1;
        }
    }
}
// 64 位输出请用 printf("%lld")
#include <iostream>
using namespace std;

const int MAX_N = 1e6 + 2;
const int MAX_M = 1e5 + 1;

int arr[MAX_N]; // n个数字
int queries[MAX_M]; // m次要查询的数

int main()
{
    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= n; i++) // 循环输入n个数字
    {
        cin >> arr[i];
    }

    for (int z = 1; z <= m; z++) // 循环进行m次查询
    {
        cin >> queries[z];

        // 二分搜索开始
        int first_occurrence = -1;//第一次出现的位置
        int left = 1, right = n;

        while (left <= right)
        {
            int mid = (left + right) / 2;

            if (arr[mid] == queries[z])//假如找到了
            {
                first_occurrence = mid;//将mid下标赋给它

                // 进行回退查找以第一次出现的位置
                while (mid > 1 && arr[mid - 1] == queries[z])
                {
                    mid--;
                    first_occurrence = mid;
                }

                break;
            }
            else if (arr[mid] > queries[z])
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        cout << first_occurrence << ' ';
    }

    return 0;
}
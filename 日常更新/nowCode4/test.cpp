#include <iostream>
using namespace std;

int a, b;

int gcd(int a, int b)
{
    int r;
    while (r = a % b)
    {
        a = b;
        b = r;
    }
    return b;
}
int main()
{
    int a, b;
    cin >> a >> b;
    cout << a * b / gcd(a, b) << endl;
    
    return 0;
}

// 64 位输出请用 printf("%lld")
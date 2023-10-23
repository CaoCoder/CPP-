#include <iostream>
#include <bitset>

using namespace std;
class BinInsert {
public:
    int binInsert(int n, int m, int j, int i) 
    {
        bitset<32> n1(n);
        bitset<32> m1(m);
        bitset<32> b(1100);

        // for(int c = 0;c <= 32; c++)
        // {
        //     if(j <=i && m1[c] == 1)
        //     {
        //         n1.set(j, 1);
        //         j++;
        //     }
        //     else if(j <=i && m1[c] == 0)
        //     {
        //         n1.set(j, 0);
        //         j++;
        //     }
        //     else
        //     {
        //         break;
        //     }
            
        // }
        // n1.set()


        cout << n1 << endl;
        cout << m1 << endl;
        cout << b << endl;
        return n1.to_ulong();
    }
};

int main()
{
    int n,m,j,i;
    // cin >> n >> m >> j >> i;
    BinInsert b;
    b.binInsert(1024,19,2,6);
    return 0;
}
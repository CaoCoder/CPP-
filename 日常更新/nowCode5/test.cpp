
#include <iostream>
#include <vector>
using namespace std;
class Board {
public:
    bool checkWon(vector<vector<int> > board) 
    {
       for(int i = 0;i < board.size(); i++)
       {
            for(auto e : board[i])
            {

                
                cout << e << " ";
            }
            cout << endl;
       }

       return 1;
    }
};

int main()
{

    Board b;
    vector<vector<int>> vv = {{1,0,1},{1,-1,-1},{1,-1,0}};

    b.checkWon(vv);
    return 0;
}
#include <iostream>
#include <string>

using namespace std;

int main() 
{
    string passwd;
    cin >> passwd;
    //判断密码长度
    int score = 0;
    if(passwd.size() <= 4)
        score += 5;
    else if(passwd.size() <=7 && passwd.size() >= 5)
        score += 10;
    else
        score += 25;

    // cout << "长度" <<score << endl;
    //判断数字
    int numCount = 0;
    for(int i = 0; i < passwd.size(); i++)
    {
        if(passwd[i] >= '0' && passwd[i] <= '9')
        {
            numCount++;
        }
    }
    if(numCount == 0)
        score += 0;
    else if(numCount == 1)
        score += 10;
    else
        score += 20;

    // cout << "数字" <<score << endl;
    //判断字母
    int a = 0;
    int A = 0;

    for(int i = 0; i < passwd.size(); i++)
    {
        if(passwd[i] >= 'a' && passwd[i] <= 'z')
        {
            a = 1;
        }
        if(passwd[i] >= 'A' && passwd[i] <= 'Z')
        {
            A = 1;
        }
    }
    if(a + A == 1)
        score+=10;
    else if(a + A == 2)
        score += 20;

//   cout << "字母" <<  score << endl;
    //判断符号
    int charCount = 0;
    for(int i = 0; i < passwd.size(); i++)
    {
        if((passwd[i] >= 0x21 && passwd[i] <= 0x2F) || 
        (passwd[i] >= 0x3A && passwd[i] <= 0x40) || 
        (passwd[i] >= 0x5B && passwd[i] <= 0x60) ||
        (passwd[i] >= 0x7B && passwd[i] <= 0x7E))
        {
            charCount++;
        }
    }
    if(charCount == 0)
        score += 0;
    else if(charCount == 1)
        score += 10;
    else
        score += 25;


    // cout << score << endl;
    //判断奖励
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    if((a + A == 1) && numCount >=1 )
        score+= 2;
    else if((a + A == 1)&& numCount >=1 && charCount >=1)
        score+= 3;
    else if((a + A == 2) && numCount >=1 && charCount >=1 )
        score+= 5;

    // cout << score << endl;
    
    if(score >= 0 && score < 25)
        cout << "VERY_WEAK";
    else if(score >= 25 && score < 50)
        cout << "WEAK";
    else if(score >= 50 && score < 60)
        cout << "AVERAGE";
    else if(score >= 60 && score < 70)
        cout << "STRONG";
    else if(score >= 70 && score < 80)
        cout << "VERY_STRONG";
    else if(score >= 80 && score < 90)
        cout << "SECURE";
    else if(score >= 90)
        cout << "VERY_SECURE";
}
// 64 位输出请用 printf("%lld")
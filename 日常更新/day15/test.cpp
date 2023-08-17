#include<iostream>
#include<memory>
using namespace std;
#include "SmartPtr.h"

// int main()
// {
//     csq::unique_ptr<int> up1(new int);
// 	//csq::unique_ptr<int> up2(up1);
// 	csq::unique_ptr<int> up2(new int);

// 	//up1 = up2;

// 	return 0;
// }
// shared_ptr：核心原理就是引用计数，记录几个对象管理这块资源，
// 析构的时候--计数，最后一个析构的对象释放资源
int main()
{
	csq::shared_ptr<int> sp1(new int);
    csq::shared_ptr<int> sp2(sp1);

	// csq::shared_ptr<int> sp3(new int);

	// sp1 = sp1;
	// sp1 = sp2;

	// sp1 = sp3;
	// sp2 = sp3;

	return 0;
}

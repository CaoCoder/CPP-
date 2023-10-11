#include <iostream>
#include<pthread.h>
#include <unistd.h>
#include <thread>
using namespace std;

void* callback1(void* args)
{
	string name = (char*)args;
	while(true)
	{
		cout << name << "; " << ::getpid() <<endl;
		sleep(1);
	}
}

void* callback2(void* args)
{
	string name = (char*)args;
	while(true)
	{
		cout << name << ": " <<::getpid() << endl;
		sleep(1);
	}
}
int main()
{
	//     std::thread t([](){
	// 	while(true)
	// 	{
	// 		cout << "线程运行" << endl;
	// 		sleep(1);
	// 	}
	// });

	// t.join();
	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, nullptr, callback1, (void*)"thread 1");
	pthread_create(&t2, nullptr, callback2, (void*)"thread 2");


	pthread_join(t1, nullptr);
	pthread_join(t2, nullptr);

	return 0;
}
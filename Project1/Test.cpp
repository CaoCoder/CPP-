
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>


#pragma once
#include "SmartPtr.h"

using namespace std;

struct ListNode
{
	int val;
	//bit::weak_ptr<ListNode> next;
	//bit::weak_ptr<ListNode> prev;

	~ListNode()
	{
		cout << "~ListNode()" << endl;
	}
};
template<class T>
struct DelArray
{
	void operator()(T* ptr)
	{
		delete[] ptr;
	}
};
int main() {
	// ¶¨ÖÆÉ¾³ýÆ÷
	//bit::shared_ptr<ListNode> sp1(new ListNode[10], DelArray<ListNode>());
	//bit::shared_ptr<ListNode> sp2(new ListNode[10], [](ListNode* ptr) {delete[] ptr; });
	bit::shared_ptr<FILE> sp3(fopen("Test.cpp", "r"), [](FILE* ptr) {fclose(ptr); });

	bit::shared_ptr<ListNode> sp4(new ListNode);
}

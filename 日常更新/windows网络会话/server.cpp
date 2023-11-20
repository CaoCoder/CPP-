/**************************************************************************
本程序为服务器端程序，通信任一方只要发送bye通信过程就自动结束。在面向连接通
信过程中服务器先与端口3000绑定，然后等待客户连接。连接成功后就可收发信息。
**************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
//#include <condefs.h>
#pragma hdrstop
//------------------------------------------------------------------------
#include <conio.h>
#include <stdio.h>
#include <winsock2.h>
//------------------------------------------------------------------------
#pragma argsused
bool InitSocket();
//---------------------------------------------------------------------
bool InitSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 0);
	//初始化Windows Sockets DLL,
	err = WSAStartup(wVersionRequested, &wsaData);//加载Winsock DLL到内存
	if (err != 0) {
		printf("没有Windows Socket动态库!\n");
		_getch();
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
		printf("需要Windows Socket 2!\n");
		_getch();
		WSACleanup(); //非winsock 2.0时，卸载Window Sockets DLL
		return false;
	}
	return true;
}
int main(int, char**)
{
	struct sockaddr addr;
	int len, off;
	bool over;
	char msg[255];
	SOCKET sock, sersock;

	if (!InitSocket()) return 0; //初始化Window Sockets DLL(ws2_32.dll)
	//创建流Socket：即传输层使用TCP
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) { //不能创建，返回
		printf("不能创建Socket!");
		_getch();
		WSACleanup(); //卸载Window Sockets DLL
		return 0;
	}
	//设置IP地址和端口号
	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:使用Internet 协议
	((sockaddr_in*)&addr)->sin_port = htons(3000); //服务器端口号3000
	((sockaddr_in*)&addr)->sin_addr.s_addr = inet_addr("127.0.0.1");//主机地址
	//把套接字与地址绑定
	bind(sock, &addr, sizeof(addr));
	//监听网络连接
	listen(sock, 1); //监听连接：1--允许等待队列的长度
	printf("等待客户连接!\n");
	len = sizeof(addr);
	//接受网络连接，生成新的套接字sersock标识这一连接
	sersock = accept(sock, &addr, &len);
	//从等待队列中检取客户，如队列空则进程挂起
	//如不空，则接受并生成新Socket以表示此连接，而原Socket继续等待新客户
	if (sersock == INVALID_SOCKET) {
		DWORD err = WSAGetLastError();
		char txt[100];
		sprintf(txt, "error when accept!---errno:%d", err);
		printf(txt);
		_getch();
		WSACleanup(); //卸载Window Sockets DLL
		return 0;
	}
	printf("有客户连接!\n输入bye通信结束\n");
	over = false;
	do {
		printf("\n等待用户输入信息......");
		len = recv(sersock, msg, 200, 0); //接收用户信息
		printf("收到信息:%s", msg);
		if (strcmp(msg, "bye") == 0) { //若收到bye就结束通信
			printf(msg); break;
		}
		printf("输入信息:"); 
		fgets(msg, sizeof(msg), stdin);
		if (strcmp(msg, "bye") == 0) over = true; //若发出bye就结束通信
		send(sersock, msg, strlen(msg) + 1, 0); //发出输入信息
	} while (!over);
	//关闭套接字
	closesocket(sersock);
	closesocket(sock);
	printf("按任一键返回");
	_getch();
	WSACleanup(); //卸载Window Sockets DLL
	return 0;
}

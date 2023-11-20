///**************************************************************************
//本程序为客户端程序。通信的任一方只要发送bye通信过程就自动结束，在面向连接的
//通信过程中，服务器首先启动，客户端启动后，先输入服务器的IP地址和端口号3000，
//再与服务器端连接，连接成功后就可收发信息。
//**************************************************************************/
//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS 
//
////#include <condefs.h>
//
//#pragma hdrstop
////------------------------------------------------------------------------
//#include <conio.h>
//#include <stdio.h>
//#include <string.h>
//#include <winsock2.h> //Windows Sockets函数定义
//#include <stdlib.h>
////------------------------------------------------------------------------
//#pragma argsused
//bool InitSocket(); //初始化Windows Sockets动态连接库
//
//int main(int, char**)
//{
//	int type;
//	SOCKET sock; //套接字
//	struct sockaddr addr; //地址
//	int len, portno;
//	int err;
//	char msg[255];
//	bool over;
//	int i;
//
//	if (!InitSocket()) return 0; //初始化Window Sockets DLL
//	type = SOCK_STREAM; //面向连接的流式Socket类型
//	sock = socket(AF_INET, type, 0); //创建支持Internet协议的流式Socket
//	if (sock == INVALID_SOCKET) { //不能创建，返回
//		printf("不能创建Socket!");
//		_getch();
//		WSACleanup(); //卸载Window Sockets DLL
//		return 0;
//	}
//	//客户端地址设定
//	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:使用Internet 协议
//	((sockaddr_in*)&addr)->sin_port = 0; //客户端端口号:0--系统分配
//	((sockaddr_in*)&addr)->sin_addr.s_addr = 0; //主机地址：0--本机
//	bind(sock, &addr, sizeof(addr)); //把地址与Socket绑定
//	//输入服务器地址
//	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:使用Internet 协议
//	printf("输入服务器地址（0.0.0.0）:");
//	fgets(msg, sizeof(msg), stdin);
//	if (msg[0] == '\0')
//		sprintf(msg, "0.0.0.0"); //如果直接回车，就采用默认设置（0.0.0.0）:本机
//	((sockaddr_in*)&addr)->sin_addr.s_addr = inet_addr(msg); //服务器地址
//	printf("输入服务器端口号(3000):");
//	fgets(msg, sizeof(msg), stdin);
//	if (msg[0] == '\0') portno = 3000; //如果直接回车，就采用默认端口号3000
//	else portno = atoi(msg);
//	((sockaddr_in*)&addr)->sin_port = htons(portno); //服务器端口号；
//	//htons()：把主机表示的短整数转化成网络字节顺序
//	len = sizeof(addr);
//	printf("与服务器连接...!");
//	err = connect(sock, (sockaddr*)&addr, len);
//	//在使用面向连接协议时(TCP)，必须与服务器连接成功后，才可通信
//	//在无连接协议(UDP)中，可以直接向对方发数据，而无需连接
//	if (err == SOCKET_ERROR) {
//		printf("连接失败!");
//		_getch();
//		WSACleanup(); //卸载Window Sockets DLL
//		return 0;
//	}
//	printf("成功连接到服务器!\n输入bye通信结束\n");
//	over = false;
//	do {
//		printf("\n输入信息:");
//		fgets(msg, sizeof(msg), stdin);
//		if (strcmp(msg, "bye") == 0) over = true; //输入“bye”则结束通信
//		send(sock, msg, strlen(msg) + 1, 0); //发送输入信息
//		if (over) break;
//		printf("\n等待服务器响应......");
//		len = recv(sock, msg, 200, 0);
//		msg[len] = 0; //接受信息
//		if (strcmp(msg, "bye") == 0) over = true; //收到“bye”则结束通信
//		printf("收到信息:");
//		printf(msg); //显示接受信息
//	} while (!over);
//	closesocket(sock);
//	printf("按任一键返回");
//	_getch();
//	WSACleanup(); //卸载Window Sockets DLL
//	return 0;
//}
////---------------------------------------------------------------------
//bool InitSocket()
//{
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int err;
//
//	wVersionRequested = MAKEWORD(2, 0); //Winsock2.0
//	//初始化Windows Sockets DLL,
//	err = WSAStartup(wVersionRequested, &wsaData); //加载Winsock DLL到内存
//	if (err != 0) {
//		printf("没有Windows Socket动态库!\n");
//		_getch();
//		return false;
//	}
//
//	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
//		printf("需要Windows Socket 2!\n");
//		_getch();
//		WSACleanup(); //非Winsock2.0时，卸载Window Sockets DLL
//		return false;
//	}
//	return true;
//}
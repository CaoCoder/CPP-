///**************************************************************************
//������Ϊ�ͻ��˳���ͨ�ŵ���һ��ֻҪ����byeͨ�Ź��̾��Զ����������������ӵ�
//ͨ�Ź����У������������������ͻ����������������������IP��ַ�Ͷ˿ں�3000��
//��������������ӣ����ӳɹ���Ϳ��շ���Ϣ��
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
//#include <winsock2.h> //Windows Sockets��������
//#include <stdlib.h>
////------------------------------------------------------------------------
//#pragma argsused
//bool InitSocket(); //��ʼ��Windows Sockets��̬���ӿ�
//
//int main(int, char**)
//{
//	int type;
//	SOCKET sock; //�׽���
//	struct sockaddr addr; //��ַ
//	int len, portno;
//	int err;
//	char msg[255];
//	bool over;
//	int i;
//
//	if (!InitSocket()) return 0; //��ʼ��Window Sockets DLL
//	type = SOCK_STREAM; //�������ӵ���ʽSocket����
//	sock = socket(AF_INET, type, 0); //����֧��InternetЭ�����ʽSocket
//	if (sock == INVALID_SOCKET) { //���ܴ���������
//		printf("���ܴ���Socket!");
//		_getch();
//		WSACleanup(); //ж��Window Sockets DLL
//		return 0;
//	}
//	//�ͻ��˵�ַ�趨
//	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:ʹ��Internet Э��
//	((sockaddr_in*)&addr)->sin_port = 0; //�ͻ��˶˿ں�:0--ϵͳ����
//	((sockaddr_in*)&addr)->sin_addr.s_addr = 0; //������ַ��0--����
//	bind(sock, &addr, sizeof(addr)); //�ѵ�ַ��Socket��
//	//�����������ַ
//	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:ʹ��Internet Э��
//	printf("�����������ַ��0.0.0.0��:");
//	fgets(msg, sizeof(msg), stdin);
//	if (msg[0] == '\0')
//		sprintf(msg, "0.0.0.0"); //���ֱ�ӻس����Ͳ���Ĭ�����ã�0.0.0.0��:����
//	((sockaddr_in*)&addr)->sin_addr.s_addr = inet_addr(msg); //��������ַ
//	printf("����������˿ں�(3000):");
//	fgets(msg, sizeof(msg), stdin);
//	if (msg[0] == '\0') portno = 3000; //���ֱ�ӻس����Ͳ���Ĭ�϶˿ں�3000
//	else portno = atoi(msg);
//	((sockaddr_in*)&addr)->sin_port = htons(portno); //�������˿ںţ�
//	//htons()����������ʾ�Ķ�����ת���������ֽ�˳��
//	len = sizeof(addr);
//	printf("�����������...!");
//	err = connect(sock, (sockaddr*)&addr, len);
//	//��ʹ����������Э��ʱ(TCP)����������������ӳɹ��󣬲ſ�ͨ��
//	//��������Э��(UDP)�У�����ֱ����Է������ݣ�����������
//	if (err == SOCKET_ERROR) {
//		printf("����ʧ��!");
//		_getch();
//		WSACleanup(); //ж��Window Sockets DLL
//		return 0;
//	}
//	printf("�ɹ����ӵ�������!\n����byeͨ�Ž���\n");
//	over = false;
//	do {
//		printf("\n������Ϣ:");
//		fgets(msg, sizeof(msg), stdin);
//		if (strcmp(msg, "bye") == 0) over = true; //���롰bye�������ͨ��
//		send(sock, msg, strlen(msg) + 1, 0); //����������Ϣ
//		if (over) break;
//		printf("\n�ȴ���������Ӧ......");
//		len = recv(sock, msg, 200, 0);
//		msg[len] = 0; //������Ϣ
//		if (strcmp(msg, "bye") == 0) over = true; //�յ���bye�������ͨ��
//		printf("�յ���Ϣ:");
//		printf(msg); //��ʾ������Ϣ
//	} while (!over);
//	closesocket(sock);
//	printf("����һ������");
//	_getch();
//	WSACleanup(); //ж��Window Sockets DLL
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
//	//��ʼ��Windows Sockets DLL,
//	err = WSAStartup(wVersionRequested, &wsaData); //����Winsock DLL���ڴ�
//	if (err != 0) {
//		printf("û��Windows Socket��̬��!\n");
//		_getch();
//		return false;
//	}
//
//	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
//		printf("��ҪWindows Socket 2!\n");
//		_getch();
//		WSACleanup(); //��Winsock2.0ʱ��ж��Window Sockets DLL
//		return false;
//	}
//	return true;
//}
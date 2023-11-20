/**************************************************************************
������Ϊ�������˳���ͨ����һ��ֻҪ����byeͨ�Ź��̾��Զ�����������������ͨ
�Ź����з���������˿�3000�󶨣�Ȼ��ȴ��ͻ����ӡ����ӳɹ���Ϳ��շ���Ϣ��
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
	//��ʼ��Windows Sockets DLL,
	err = WSAStartup(wVersionRequested, &wsaData);//����Winsock DLL���ڴ�
	if (err != 0) {
		printf("û��Windows Socket��̬��!\n");
		_getch();
		return false;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
		printf("��ҪWindows Socket 2!\n");
		_getch();
		WSACleanup(); //��winsock 2.0ʱ��ж��Window Sockets DLL
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

	if (!InitSocket()) return 0; //��ʼ��Window Sockets DLL(ws2_32.dll)
	//������Socket���������ʹ��TCP
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET) { //���ܴ���������
		printf("���ܴ���Socket!");
		_getch();
		WSACleanup(); //ж��Window Sockets DLL
		return 0;
	}
	//����IP��ַ�Ͷ˿ں�
	((sockaddr_in*)&addr)->sin_family = AF_INET; //AF_INET:ʹ��Internet Э��
	((sockaddr_in*)&addr)->sin_port = htons(3000); //�������˿ں�3000
	((sockaddr_in*)&addr)->sin_addr.s_addr = inet_addr("127.0.0.1");//������ַ
	//���׽������ַ��
	bind(sock, &addr, sizeof(addr));
	//������������
	listen(sock, 1); //�������ӣ�1--����ȴ����еĳ���
	printf("�ȴ��ͻ�����!\n");
	len = sizeof(addr);
	//�����������ӣ������µ��׽���sersock��ʶ��һ����
	sersock = accept(sock, &addr, &len);
	//�ӵȴ������м�ȡ�ͻ�������п�����̹���
	//�粻�գ�����ܲ�������Socket�Ա�ʾ�����ӣ���ԭSocket�����ȴ��¿ͻ�
	if (sersock == INVALID_SOCKET) {
		DWORD err = WSAGetLastError();
		char txt[100];
		sprintf(txt, "error when accept!---errno:%d", err);
		printf(txt);
		_getch();
		WSACleanup(); //ж��Window Sockets DLL
		return 0;
	}
	printf("�пͻ�����!\n����byeͨ�Ž���\n");
	over = false;
	do {
		printf("\n�ȴ��û�������Ϣ......");
		len = recv(sersock, msg, 200, 0); //�����û���Ϣ
		printf("�յ���Ϣ:%s", msg);
		if (strcmp(msg, "bye") == 0) { //���յ�bye�ͽ���ͨ��
			printf(msg); break;
		}
		printf("������Ϣ:"); 
		fgets(msg, sizeof(msg), stdin);
		if (strcmp(msg, "bye") == 0) over = true; //������bye�ͽ���ͨ��
		send(sersock, msg, strlen(msg) + 1, 0); //����������Ϣ
	} while (!over);
	//�ر��׽���
	closesocket(sersock);
	closesocket(sock);
	printf("����һ������");
	_getch();
	WSACleanup(); //ж��Window Sockets DLL
	return 0;
}

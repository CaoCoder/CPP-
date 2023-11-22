#include <iostream>
#include <ctime>
#include <string>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <io.h>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <mmsystem.h>
using namespace std;

#define GRASS 0
#define GRAVE1 1
#define GRAVE2 2
#define GRAVE3 3
#define GRAVE4 4
#define GRAVE5 5
#define GRAVE6 6
#define GRAVE7 7
#define GRAVE8 8
#define POTATO 9
#define POTATOMINE 10
#define POTATOBOOM 11
#define GRAVEBUSTER_GRAVE1 12
#define GRAVEBUSTER_GRAVE2 13
#define GRAVEBUSTER_GRAVE3 14
#define GRAVEBUSTER_GRAVE4 15
#define GRAVEBUSTER_GRAVE5 16
#define GRAVEBUSTER_GRAVE6 17
#define GRAVEBUSTER_GRAVE7 18
#define GRAVEBUSTER_GRAVE8 19
#define ICESHROOM 20
#define NORMALZOMBIE 21
#define CONEHEADZOMBIE 22
#define BUCKETHEADZOMBIE 16

//非游戏参数
int zombieNum=0;
int plantNum=0;
int sunNum=0;
int bangNum=0;
double groanFrequency = 0.0005;
IMAGE potatoBoom;
IMAGE potato;
IMAGE grave[8];
IMAGE hammer[13];
IMAGE tmpImg;
IMAGE tmpImg2;
IMAGE potaotoMinePictures[8];
IMAGE iceshroomPictures[11];
IMAGE gravebusterPictures[28];
IMAGE sunPictures[22];
IMAGE normalZombieWalkPictures[47];
IMAGE normalZombieEmergePictures[20];
IMAGE normalZombieEatPictures[10];
IMAGE coneheadZombieWalkPictures[47];
IMAGE coneheadZombieEmergePictures[20];
IMAGE coneheadZombieEatPictures[10];
IMAGE bucketheadZombieWalkPictures[47];
IMAGE bucketheadZombieEmergePictures[20];
IMAGE bucketheadZombieEatPictures[10];
IMAGE plantsBar;
IMAGE menu;
IMAGE background;
IMAGE selectID;
IMAGE iceTrap;
IMAGE snow;
IMAGE lawnmower;
IMAGE loseGame;
IMAGE winGame;
IMAGE bang;
ExMessage mousemsg;

struct coordinate
{
	int x;
	int y;
};

enum CURSORFLAG
{
	Chammer, 
	CpotatoMine,
	Ciceshroom,
	Cgravebuster
};

coordinate xys[32][32];
CURSORFLAG cursor;
RECT rect = { 0, 500, 820, 600 };
char sunshineNum[10];
char username[200];
vector<string> files;

class Bang
{
public:
	int No;
	int x;
	int y;
	int countDown;

	Bang(int x,int y)
	{
		No = bangNum;
		bangNum++;
		this->x = x;
		this->y = y;
		countDown = 20;
	}
};

class Sun
{
public:
	int x;
	int y;
	int frame;
	int No;
	int changeFrameCountDown;
	int goToCount;
	int goToCountFrame;
	int tempX;
	int tempY;

	Sun(int x,int y)
	{
		frame = 0;
		No=sunNum;
		sunNum++;
		this->x = x;
		this->y = y;
		this->tempX = x;
		this->tempY = y;
		changeFrameCountDown = 5;
		goToCount = 0;
		goToCountFrame = 10;
	}
};

class Plant
{
public:
	int type;
	int HP;
	int frameNo;
	int No;
	int x;
	int y;
	int changeFrameCountDown;

	Plant()
	{
		No = plantNum;
		plantNum++;
		changeFrameCountDown = 5;
		HP = 6;
	}
	~Plant(){}
};

// 土豆雷
class PotatoMine : public Plant
{
public:
	int underCountDown = 400;
	int boomCountDown = 50;

	PotatoMine()
	{
		frameNo = 0;
		type = POTATOMINE;
	}
};

// 墓碑吞噬者
class GraveBuster : public Plant
{
public:
	GraveBuster()
	{
		frameNo = 1;
		type = GRAVEBUSTER_GRAVE1;
	}
};

// 寒冰菇
class IceShroom : public Plant
{
public:
	int frozenCountDown = 200;
	int slowingCountDown = 1000;

	IceShroom()
	{
		frameNo = 0;
		type = ICESHROOM;
	}
};


class Zombie
{
public:
	int HP;
	int row;
	int location;
	int emerge1walk2eat3;
	int frameNo;
	int height;
	int No;
	int changeFrameCountDown;
	int isFrozen;
	int isSlowed;
	int type;

	Zombie()
	{
		No = zombieNum;
		zombieNum++;
		isFrozen = 0;
		isSlowed = 0;
		height = 115;			// 僵尸图像高度
		frameNo = 19;			// 表示播放到第几帧
		emerge1walk2eat3 = 1;	// 正在冒出来用 1 表示，正在行走用 2 表示，正在吃植物用 3 表示
		changeFrameCountDown = 10;
	}
};
class NormalZombie : public Zombie
{
public:
	NormalZombie()
	{
		HP = 1;
		type = NORMALZOMBIE;
	}
};

class ConeheadZombie : public Zombie
{
public:
	ConeheadZombie()
	{
		HP = 2;
		type = CONEHEADZOMBIE;
	}
};

class BucketheadZombie : public Zombie
{
public:
	BucketheadZombie()
	{
		HP = 3;
		type = BUCKETHEADZOMBIE;
	}
};

class Lawnmower
{
public:
	int location = -20;
	int isActivated = 0;
	int isOut = 0;
};

template<class T>
class Node
{
public:
	T* content;
	Node* next = NULL;
	Node(T* t)
	{
		content = t;
	}
};

template<class T>
class LinkList
{
public:
	Node<T>* head;  
	Node<T>* tail;

	LinkList()
	{
		head = NULL;
		tail = NULL;
	};

	LinkList(Node<T> node)
	{ 
		head = node; 
		tail = node; 
	};

	~LinkList()
	{ 
		DeleteAllNode();
	}     

	void InsertNode(T* t)
	{
		Node<T>* node=new Node<T>(t);
		if (head == NULL)
		{
			head = node;
			tail = node;
		}
		else
		{
			tail->next = node;
			tail = node;
		}
	};

	void DeleteNode(int No)
	{
		Node<T>* cur = head,*pre=NULL;
		while (cur != NULL && cur->content->No != No)
		{
			pre = cur;
			cur = cur->next;
		}

		if (pre == NULL)
		{
			head = cur->next;
		}
		else if (cur == NULL)
		{
			cout << "没有找到符合条件的结点！" << endl;
			return;
		}
		else
		{
			pre->next = cur->next;
		}

		if (cur == tail)
		{
			tail = pre;
		}
		delete cur;
	};

	void DeleteAllNode()
	{
		Node<T>* cur = head,*pre=NULL;
		while (tail != NULL)
		{
			pre = cur;
			cur = cur->next;
			DeleteNode(pre->content->No);
		}
	};
};
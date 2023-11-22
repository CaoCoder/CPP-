/*
 * 程序名称：植物大战僵尸之锤僵尸小游戏
 * 作　　者：JiHe (QQ_3470498828)
 * 最后修改：2022-1-14
 * 暂未实现功能：僵尸遇到植物停下开始吃及其音效
 */

#include "head.h"
#pragma warning (disable:4996)
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment( lib, "winmm.lib")

// 游戏参数
int mapState[32][32];		// 地图状态。0：空，1：墓碑，2：地雷（没出土），3：地雷（已出土），4：寒冰菇
int currentSunshine;
LinkList<Sun> suns;
LinkList<Plant> plants;
LinkList<Zombie> zombies;
LinkList<Bang> bangs;
Lawnmower* lawnmowers[5];
double normalfrequency;
double coneheadfrequency;
double bucketheadfrequency;
double SunsFrequency;
int isNewGame;
int isHitting;
int hammerRadius;
int drawingHint;
int hintCountDown;
int snowCountDown;
int graveNum;
int Win1Lose2;

void init()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mapState[i][j] = GRASS;
		}
	}

	currentSunshine = 0;
	plants.DeleteAllNode();
	zombies.DeleteAllNode();
	suns.DeleteAllNode();
	bangs.DeleteAllNode();

	for (int i = 0; i < 5; i++)
	{
		lawnmowers[i] = new Lawnmower();
	}

	normalfrequency = 0.002;
	coneheadfrequency = 0.0025;
	bucketheadfrequency = 0.0028;
	SunsFrequency = 0.05;
	isNewGame = 1;
	isHitting = 0;
	hammerRadius = 0;
	drawingHint = 0;
	hintCountDown = 70;
	snowCountDown = 0;
	graveNum = 0;
	Win1Lose2 = 0;
}

void getFiles(string path)
{
	files.clear();
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	files.erase(files.begin());
	files.erase(files.begin());
}

void readArchive(char name[])
{
	init();
	char path[] = "./archives/", tmppath[200] = { 0 };
	strcat(strcat(tmppath, path), name);
	FILE* fp = fopen(tmppath, "rb");
	::fread(&mapState, sizeof(mapState), 1, fp);

	for (int i = 0; i < 5; i++)
	{
		lawnmowers[i] = new Lawnmower();
		::fread(&lawnmowers[i]->location, sizeof(int), 1, fp);
		::fread(&lawnmowers[i]->isActivated, sizeof(int), 1, fp);
		::fread(&lawnmowers[i]->isOut, sizeof(int), 1, fp);
	}

	fread(&currentSunshine,		sizeof(int),	1, fp);
	fread(&normalfrequency,		sizeof(double),	1, fp);
	fread(&coneheadfrequency,	sizeof(double),	1, fp);
	fread(&bucketheadfrequency,	sizeof(double),	1, fp);
	fread(&SunsFrequency,		sizeof(double),	1, fp);
	fread(&isNewGame,			sizeof(int),	1, fp);
	fread(&isHitting,			sizeof(int),	1, fp);
	fread(&hammerRadius,		sizeof(int),	1, fp);
	fread(&drawingHint,			sizeof(int),	1, fp);
	fread(&hintCountDown,		sizeof(int),	1, fp);
	fread(&snowCountDown,		sizeof(int),	1, fp);
	fread(&graveNum,			sizeof(int),	1, fp);
	fread(&Win1Lose2,			sizeof(int),	1, fp);

	int separator;
	while (1)
	{
		fread(&separator, sizeof(int), 1, fp);
		if (separator!=1234567)
		{
			Zombie* tmpZombie = new Zombie();
			fseek(fp, -(int)sizeof(int), SEEK_CUR);
			fread(&tmpZombie->HP,					sizeof(int), 1, fp);
			fread(&tmpZombie->row,					sizeof(int), 1, fp);
			fread(&tmpZombie->location,				sizeof(int), 1, fp);
			fread(&tmpZombie->emerge1walk2eat3,		sizeof(int), 1, fp);
			fread(&tmpZombie->frameNo,				sizeof(int), 1, fp);
			fread(&tmpZombie->height,				sizeof(int), 1, fp);
			fread(&tmpZombie->No,					sizeof(int), 1, fp);
			fread(&tmpZombie->changeFrameCountDown,	sizeof(int), 1, fp);
			fread(&tmpZombie->isFrozen,				sizeof(int), 1, fp);
			fread(&tmpZombie->isSlowed,				sizeof(int), 1, fp);
			fread(&tmpZombie->type,					sizeof(int), 1, fp);
			zombies.InsertNode(tmpZombie);
		}
		else break;
	}

	int tmpPlantType;
	while (1)
	{
		fread(&separator, sizeof(int), 1, fp);
		if (separator!=7654321)
		{
			fseek(fp, -(int)sizeof(int), SEEK_CUR);
			fread(&tmpPlantType, sizeof(int), 1, fp);
			switch (tmpPlantType)
			{
				case POTATOMINE:
				{
					PotatoMine* tmpPotatoMine = new PotatoMine();
					tmpPotatoMine->type = tmpPlantType;
					fread(&tmpPotatoMine->frameNo, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->No, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->x, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->y, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->changeFrameCountDown, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->underCountDown, sizeof(int), 1, fp);
					fread(&tmpPotatoMine->boomCountDown, sizeof(int), 1, fp);
					plants.InsertNode(tmpPotatoMine);
					break;
				}
				case GRAVEBUSTER_GRAVE1:
				{
					GraveBuster* tmpGraveBuster = new GraveBuster();
					tmpGraveBuster->type = tmpPlantType;
					fread(&tmpGraveBuster->frameNo, sizeof(int), 1, fp);
					fread(&tmpGraveBuster->No, sizeof(int), 1, fp);
					fread(&tmpGraveBuster->x, sizeof(int), 1, fp);
					fread(&tmpGraveBuster->y, sizeof(int), 1, fp);
					fread(&tmpGraveBuster->changeFrameCountDown, sizeof(int), 1, fp);
					plants.InsertNode(tmpGraveBuster);
					break;
				}
				case ICESHROOM:
				{
					IceShroom* tmpIceShroom = new IceShroom();
					tmpIceShroom->type = tmpPlantType;
					fread(&tmpIceShroom->frameNo, sizeof(int), 1, fp);
					fread(&tmpIceShroom->No, sizeof(int), 1, fp);
					fread(&tmpIceShroom->x, sizeof(int), 1, fp);
					fread(&tmpIceShroom->y, sizeof(int), 1, fp);
					fread(&tmpIceShroom->changeFrameCountDown, sizeof(int), 1, fp);
					fread(&tmpIceShroom->frozenCountDown, sizeof(int), 1, fp);
					fread(&tmpIceShroom->slowingCountDown, sizeof(int), 1, fp);
					plants.InsertNode(tmpIceShroom);
					break;
				}
			}
		}
		else break;
	}

	while (1)
	{
		fread(&separator, sizeof(int), 1, fp);
		if (separator != 357421)
		{
			Bang* tmpBang = new Bang(0,0);
			fseek(fp, -(int)sizeof(int), SEEK_CUR);
			fread(&tmpBang->No, sizeof(int), 1, fp);
			fread(&tmpBang->x, sizeof(int), 1, fp);
			fread(&tmpBang->y, sizeof(int), 1, fp);
			fread(&tmpBang->countDown, sizeof(int), 1, fp);
			bangs.InsertNode(tmpBang);
		}
		else
			break;
	}

	while (fread(&separator,sizeof(int),1,fp))
	{
		Sun* tmpSun = new Sun(0, 0);
		fread(&tmpSun->x, sizeof(int), 1, fp);
		fread(&tmpSun->y, sizeof(int), 1, fp);
		fread(&tmpSun->frame, sizeof(int), 1, fp);
		fread(&tmpSun->No, sizeof(int), 1, fp);
		fread(&tmpSun->changeFrameCountDown, sizeof(int), 1, fp);
		fread(&tmpSun->goToCount, sizeof(int), 1, fp);
		fread(&tmpSun->goToCountFrame, sizeof(int), 1, fp);
		fread(&tmpSun->tempX, sizeof(int), 1, fp);
		fread(&tmpSun->tempY, sizeof(int), 1, fp);
		suns.InsertNode(tmpSun);
	}
	fclose(fp);
}

void writeArchive(char name[])
{
	char path[] = "./archives/", tmppath[200] = { 0 };
	strcat(strcat(tmppath, path), name);
	FILE* fp = fopen(tmppath, "wb");
	::fwrite(mapState, sizeof(mapState), 1, fp);

	for (int i = 0; i < 5; i++)
	{
		::fwrite(&lawnmowers[i]->location, sizeof(int), 1, fp);
		::fwrite(&lawnmowers[i]->isActivated, sizeof(int), 1, fp);
		::fwrite(&lawnmowers[i]->isOut, sizeof(int), 1, fp);
	}

	::fwrite(&currentSunshine, sizeof(int), 1, fp);
	::fwrite(&normalfrequency, sizeof(double), 1, fp);
	::fwrite(&coneheadfrequency, sizeof(double), 1, fp);
	::fwrite(&bucketheadfrequency, sizeof(double), 1, fp);
	::fwrite(&SunsFrequency, sizeof(double), 1, fp);
	::fwrite(&isNewGame, sizeof(int), 1, fp);
	::fwrite(&isHitting, sizeof(int), 1, fp);
	::fwrite(&hammerRadius, sizeof(int), 1, fp);
	::fwrite(&drawingHint, sizeof(int), 1, fp);
	::fwrite(&hintCountDown, sizeof(int), 1, fp);
	::fwrite(&snowCountDown, sizeof(int), 1, fp);
	::fwrite(&graveNum, sizeof(int), 1, fp);
	::fwrite(&Win1Lose2, sizeof(int), 1, fp);

	Node<Zombie>* curZombie = zombies.head;
	while (curZombie != NULL)
	{
		Zombie* zombie = curZombie->content;
		::fwrite(&zombie->HP, sizeof(int), 1, fp);
		::fwrite(&zombie->row, sizeof(int), 1, fp);
		::fwrite(&zombie->location, sizeof(int), 1, fp);
		::fwrite(&zombie->emerge1walk2eat3, sizeof(int), 1, fp);
		::fwrite(&zombie->frameNo, sizeof(int), 1, fp);
		::fwrite(&zombie->height, sizeof(int), 1, fp);
		::fwrite(&zombie->No, sizeof(int), 1, fp);
		::fwrite(&zombie->changeFrameCountDown, sizeof(int), 1, fp);
		::fwrite(&zombie->isFrozen, sizeof(int), 1, fp);
		::fwrite(&zombie->isSlowed, sizeof(int), 1, fp);
		::fwrite(&zombie->type, sizeof(int), 1, fp);
		curZombie = curZombie->next;
	}

	int separator1 = 1234567;
	::fwrite(&separator1, sizeof(int), 1, fp);
	Node<Plant>* curPlant = plants.head;
	while (curPlant != NULL)
	{
		switch (curPlant->content->type)
		{
			case POTATOMINE:
			{
				PotatoMine* potatoMine = static_cast<PotatoMine*>(curPlant->content);
				::fwrite(&potatoMine->type, sizeof(int), 1, fp);
				::fwrite(&potatoMine->frameNo, sizeof(int), 1, fp);
				::fwrite(&potatoMine->No, sizeof(int), 1, fp);
				::fwrite(&potatoMine->x, sizeof(int), 1, fp);
				::fwrite(&potatoMine->y, sizeof(int), 1, fp);
				::fwrite(&potatoMine->changeFrameCountDown, sizeof(int), 1, fp);
				::fwrite(&potatoMine->underCountDown, sizeof(int), 1, fp);
				::fwrite(&potatoMine->boomCountDown, sizeof(int), 1, fp);
				break;
			}
			case ICESHROOM:
			{
				IceShroom* iceShroom = static_cast<IceShroom*>(curPlant->content);
				::fwrite(&iceShroom->type, sizeof(int), 1, fp);
				::fwrite(&iceShroom->frameNo, sizeof(int), 1, fp);
				::fwrite(&iceShroom->No, sizeof(int), 1, fp);
				::fwrite(&iceShroom->x, sizeof(int), 1, fp);
				::fwrite(&iceShroom->y, sizeof(int), 1, fp);
				::fwrite(&iceShroom->changeFrameCountDown, sizeof(int), 1, fp);
				::fwrite(&iceShroom->frozenCountDown, sizeof(int), 1, fp);
				::fwrite(&iceShroom->slowingCountDown, sizeof(int), 1, fp);
				break;
			}
			case GRAVEBUSTER_GRAVE1:
			{
				GraveBuster* graveBuster = static_cast<GraveBuster*>(curPlant->content);
				::fwrite(&graveBuster->type, sizeof(int), 1, fp);
				::fwrite(&graveBuster->frameNo, sizeof(int), 1, fp);
				::fwrite(&graveBuster->No, sizeof(int), 1, fp);
				::fwrite(&graveBuster->x, sizeof(int), 1, fp);
				::fwrite(&graveBuster->y, sizeof(int), 1, fp);
				::fwrite(&graveBuster->changeFrameCountDown, sizeof(int), 1, fp);
				break;
			}
		}
		curPlant = curPlant->next;
	}

	int separator2 = 7654321;
	::fwrite(&separator2, sizeof(int), 1, fp);
	Node<Bang>* curBang = bangs.head;
	while (curBang!= NULL)
	{
		Bang* bang = curBang->content;
		::fwrite(&bang->No, sizeof(int), 1, fp);
		::fwrite(&bang->x, sizeof(int), 1, fp);
		::fwrite(&bang->y, sizeof(int), 1, fp);
		::fwrite(&bang->countDown, sizeof(int), 1, fp);
		curBang = curBang->next;
	}

	int separator3 = 357421;
	::fwrite(&separator3, sizeof(int), 1, fp);
	Node<Sun>* curSun = suns.head;
	while (curSun != NULL)
	{
		Sun* sun = curSun->content;
		::fwrite(&sun->x, sizeof(int), 1, fp);
		::fwrite(&sun->y, sizeof(int), 1, fp);
		::fwrite(&sun->frame, sizeof(int), 1, fp);
		::fwrite(&sun->No, sizeof(int), 1, fp);
		::fwrite(&sun->changeFrameCountDown, sizeof(int), 1, fp);
		::fwrite(&sun->goToCount, sizeof(int), 1, fp);
		::fwrite(&sun->goToCountFrame, sizeof(int), 1, fp);
		::fwrite(&sun->tempX, sizeof(int), 1, fp);
		::fwrite(&sun->tempY, sizeof(int), 1, fp);
		curSun = curSun->next;
	}

	fclose(fp);
}

// 精确延时函数(可以精确到 1ms，精度 ±1ms)
// by yangw80<yw80@qq.com>, 2011-5-4
void HpSleep(int ms)
{
	static clock_t oldclock = clock();		// 静态变量，记录上一次 tick

	oldclock += ms * CLOCKS_PER_SEC / 1000;	// 更新 tick

	if (clock() > oldclock)					// 如果已经超时，无需延时
		oldclock = clock();
	else
		while (clock() < oldclock)			// 延时
			Sleep(1);						// 释放 CPU 控制权，降低 CPU 占用率
//			Sleep(0);						// 更高精度、更高 CPU 占用率
}

void transparentImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void paintPlantsAndGraves()
{
	// 画植物和墓碑
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			switch (mapState[i][j])
			{
			case GRASS:
				break;
			case GRAVE1:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[0]);
				break;
			}
			case GRAVE2:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[1]);
				break;
			}
			case GRAVE3:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[2]);
				break;
			}
			case GRAVE4:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[3]);
				break;
			}
			case GRAVE5:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[4]);
				break;
			}
			case GRAVE6:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[5]);
				break;
			}
			case GRAVE7:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[6]);
				break;
			}
			case GRAVE8:
			{
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[7]);
				break;
			}
			case POTATO:
			{
				transparentImage(NULL, xys[i][j].x, xys[i][j].y + 37, &potato);
				Node<Plant>* cur = plants.head;
				while (cur != NULL)
				{
					if (cur->content->x == i && cur->content->y == j)
					{
						break;
					}
					else cur = cur->next;
				}
				if (cur != NULL)
				{
					PotatoMine* potato = static_cast<PotatoMine*>(cur->content);
					potato->underCountDown--;
					if (potato->underCountDown == 0)
					{
						mapState[i][j] = POTATOMINE;
						mciSendString("play ./Music/dirt_rise.mp3 from 0 ", 0, 0, 0);
					}
				}
				break;
			}
			case POTATOMINE:
			{
				Node<Plant>* cur = plants.head;
				while (cur != NULL)
				{
					if (cur->content->x == i && cur->content->y == j)break;
					else cur = cur->next;
				}                
				if (cur != NULL)
				{
					transparentImage(NULL, xys[i][j].x, xys[i][j].y + 40, &potaotoMinePictures[cur->content->frameNo]);
					cur->content->changeFrameCountDown--;
					if (cur->content->changeFrameCountDown == 0)
					{
						cur->content->changeFrameCountDown = 20;
						cur->content->frameNo++;
						if (cur->content->frameNo == 7)
						{
							cur->content->frameNo = 0;
						}
					}
				}                   
				break;
			}
			case POTATOBOOM:
			{
				transparentImage(NULL, xys[i][j].x - 25, xys[i][j].y + 20, &potatoBoom);
				Node<Plant>* cur = plants.head;
				while (cur != NULL)
				{
					if (cur->content->x == i && cur->content->y == j)break;
					else cur = cur->next;
				}

				if (cur != NULL)
				{
					PotatoMine* potato = static_cast<PotatoMine*>(cur->content);
					potato->boomCountDown--;
					if (potato->boomCountDown == 0)
					{
						plants.DeleteNode(potato->No);
						mapState[i][j] = GRASS;
					}
				}            

				break;
			}
			case GRAVEBUSTER_GRAVE1:
				transparentImage(NULL, xys[i][j].x-5, xys[i][j].y, &grave[0]);
				goto label;
			case GRAVEBUSTER_GRAVE2:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[1]);
				goto label;
			case GRAVEBUSTER_GRAVE3:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[2]);
				goto label;
			case GRAVEBUSTER_GRAVE4:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[3]);
				goto label;
			case GRAVEBUSTER_GRAVE5:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[4]);
				goto label;
			case GRAVEBUSTER_GRAVE6:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[5]);
				goto label;
			case GRAVEBUSTER_GRAVE7:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[6]);
				goto label;
			case GRAVEBUSTER_GRAVE8:
				transparentImage(NULL, xys[i][j].x - 5, xys[i][j].y, &grave[7]);
				goto label;
			{
			label:
				Node<Plant>* cur = plants.head;
				while (cur != NULL)
				{
					if (cur->content->x == i && cur->content->y == j)break;
					else cur = cur->next;
				}
				if (cur != NULL)
				{
					transparentImage(NULL, xys[i][j].x - 10, xys[i][j].y - 10, &gravebusterPictures[cur->content->frameNo]);
					cur->content->changeFrameCountDown--;
					if (cur->content->changeFrameCountDown == 0)
					{
						cur->content->changeFrameCountDown = 10;
						cur->content->frameNo++;
						if (cur->content->frameNo > 27)
						{
							plants.DeleteNode(cur->content->No);
							mapState[i][j] = GRASS;
							graveNum--;
							if (graveNum == 5)
							{
								SunsFrequency = 0.2;
								normalfrequency = 0.003;
								coneheadfrequency = 0.0035;
								bucketheadfrequency = 0.0038;
							}
							else if (graveNum == 3)
							{
								SunsFrequency = 0.4;
								normalfrequency=0.006;
								coneheadfrequency=0.0065;
								bucketheadfrequency=0.0068;
							}
						}
					}
				}        
				break;
			}
			case ICESHROOM:
			{
				Node<Plant>* cur = plants.head;
				while (cur != NULL)
				{
					if (cur->content->x == i && cur->content->y == j)break;
					else cur = cur->next;
				}
				if (cur != NULL)
				{
					transparentImage(NULL, xys[i][j].x, xys[i][j].y + 15, &iceshroomPictures[cur->content->frameNo]);
					cur->content->changeFrameCountDown--;
					if (cur->content->changeFrameCountDown == 0)
					{
						cur->content->changeFrameCountDown = 8;
						cur->content->frameNo++;
						if (cur->content->frameNo > 10)
						{
							plants.DeleteNode(cur->content->No);
							mciSendString("play ./Music/shoop.mp3 from 0 ", 0, 0, 0);
							mapState[i][j] = 0;
							snowCountDown = 20;
							Node<Zombie>* cur = zombies.head;
							while (cur != NULL)
							{
								cur->content->isFrozen = 200;
								cur = cur->next;
							}
						}
					}
				}              
				break;
			}
			}
		}
	}
}

void addIce(IMAGE* targetImage, IMAGE* srcImage, int addRed = 0, int addGreen = 0, int addBlue = 50)
{
	int srcImgWidth = srcImage->getwidth(), srcImgHeight = srcImage->getheight();
	targetImage->Resize(srcImgWidth, srcImgHeight);
	DWORD* pTargetBuffer = GetImageBuffer(targetImage);
	DWORD* pSrcBuffer = GetImageBuffer(srcImage);
	int allPixel = srcImgHeight * srcImgWidth;

#define RGBA(r, g, b, a) ((b) + (g << 8) + (r << 16) + (a << 24))
	for (int i = 0; i < allPixel; ++i)
	{
		UCHAR r = (UCHAR)GetRValue(pSrcBuffer[i]);
		UCHAR g = (UCHAR)GetGValue(pSrcBuffer[i]);
		UCHAR b = (UCHAR)GetBValue(pSrcBuffer[i]);
		r = r + addRed;
		r = r > 255 ? 255 : r;
		g = g + addGreen;
		g = g > 255 ? 255 : g;
		b = b + addBlue;
		b = b > 255 ? 255 : b;
		pTargetBuffer[i] = (DWORD)RGBA(r, g, b, pSrcBuffer[i] >> 24);
	}
}

void generateSunshine(int x, int y)
{
	// 一定概率产生3个阳光
	double p = rand() / (double)RAND_MAX;
	if (p < SunsFrequency)
	{
		Sun* sunshine[3];
		for (int i = 0; i < 3; i++)
		{
			sunshine[i] = new Sun(x + 80 + rand() % 100 - 50, y + 60 + rand() % 50 - 25);
			suns.InsertNode(sunshine[i]);
		}
	}
}

void paintZombies()
{
	// 画僵尸
	Node<Zombie> *cur = zombies.head, *next = NULL;
	while (cur != NULL)
	{
		Zombie* zombieptr = cur->content;
		if (zombieptr->location < -150)
		{
			cur = cur->next;
			zombies.DeleteNode(zombieptr->No);
			continue;
		}
	   
		if (zombieptr->emerge1walk2eat3 == 1)
		{
			if (zombieptr->type == NORMALZOMBIE)
				tmpImg=normalZombieEmergePictures[zombieptr->frameNo];
			else if (zombieptr->type == CONEHEADZOMBIE)
				tmpImg = coneheadZombieEmergePictures[zombieptr->frameNo];
			else
				tmpImg = bucketheadZombieEmergePictures[zombieptr->frameNo];

			transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg);
			zombieptr->frameNo--;
			if (zombieptr->frameNo == 0)
			{
				zombieptr->emerge1walk2eat3 = 2;
			}
		}
		else if (zombieptr->emerge1walk2eat3 == 2)
		{
			if (zombieptr->type == NORMALZOMBIE)
				tmpImg = normalZombieWalkPictures[zombieptr->frameNo];
			else if (zombieptr->type == CONEHEADZOMBIE)
				tmpImg = coneheadZombieWalkPictures[zombieptr->frameNo];
			else
				tmpImg = bucketheadZombieWalkPictures[zombieptr->frameNo];

			if (zombieptr->isFrozen)
			{
				addIce(&tmpImg2, &tmpImg);
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg2);
			}
			else if (zombieptr->isSlowed)
			{
				addIce(&tmpImg2, &tmpImg);
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg2);
				zombieptr->changeFrameCountDown -= 1;
				if (zombieptr->changeFrameCountDown <= 0)
				{
					zombieptr->changeFrameCountDown = 6;
					zombieptr->location -= 1; 
					zombieptr->frameNo++;
					if (zombieptr->frameNo > 46)
					{
						zombieptr->frameNo = 0;
					}
				}
			}
			else
			{
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg);
				zombieptr->changeFrameCountDown -= 2;
				if (zombieptr->changeFrameCountDown <= 0)
				{
					zombieptr->changeFrameCountDown = 6;
					zombieptr->location -= 2;
					zombieptr->frameNo++;
					if (zombieptr->frameNo > 46)
					{
						zombieptr->frameNo = 0;
					}
				}
			}
		}
		else if (zombieptr->emerge1walk2eat3 == 3)
		{
			/*if (zombieptr->type == NORMALZOMBIE)tmpImg = normalZombieEatPictures[zombieptr->frameNo];
			else if (zombieptr->type == CONEHEADZOMBIE)tmpImg = coneheadZombieEatPictures[zombieptr->frameNo];
			else tmpImg = bucketheadZombieEatPictures[zombieptr->frameNo];
			if (zombieptr->isFrozen) {
				addIce(&tmpImg2, &tmpImg);
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg2);
			}
			else if (zombieptr->isSlowed) {
				addIce(&tmpImg2, &tmpImg);
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg2);
				zombieptr->changeFrameCountDown -= 1;
				if (zombieptr->changeFrameCountDown <= 0) {
					zombieptr->changeFrameCountDown = 6;
					zombieptr->location -= 1;
					zombieptr->frameNo++;
					if (zombieptr->frameNo > 46) {
						zombieptr->frameNo = 0;
					}
				}
			}
			else {
				transparentImage(NULL, zombieptr->location, xys[zombieptr->row][0].y - 40, &tmpImg);
				zombieptr->changeFrameCountDown -= 2;
				if (zombieptr->changeFrameCountDown <= 0) {
					zombieptr->changeFrameCountDown = 25;
					zombieptr->frameNo++;
				}
			}*/
		}
		// 判断是否冻住
		if (zombieptr->isFrozen > 0)
		{
			zombieptr->isFrozen--;
			if (zombieptr->isFrozen == 0)	zombieptr->isSlowed = 400;
			transparentImage(NULL, zombieptr->location+100, xys[zombieptr->row][0].y+70, &iceTrap);           
		}
		// 判断是否减速
		if (zombieptr->isSlowed > 0)
		{
			zombieptr->isSlowed--;
		}
		// 如果僵尸走到最左边且此行有除草机
		if (zombieptr->location < -50 && lawnmowers[zombieptr->row]->isOut == 0)
		{
			lawnmowers[zombieptr->row]->isActivated = 1;
			mciSendString("play lawnmower from 0", 0, 0, 0);
		}
		//// 如果僵尸前面有植物
		//Node<Plant>* curPlant = plants.head,*pre;
		//while (curPlant != NULL)
		//{
		//	Plant* plant = curPlant->content;
		//	if (zombieptr->row == plant->x
		//		&& zombieptr->location < xys[plant->x][plant->y].x- 25
		//		&& zombieptr->location > xys[plant->x][plant->y-1].x-25)
		//	{
		//		if (zombieptr->emerge1walk2eat3 == 2)
		//		{
		//			zombieptr->emerge1walk2eat3 = 3;
		//			zombieptr->frameNo = 0;
		//			mciSendString("play ./Music/chomp.mp3 from 0", 0, 0, 0);
		//		}
		//		if (zombieptr->emerge1walk2eat3 == 3 && zombieptr->frameNo > 9)
		//		{
		//			zombieptr->frameNo = 0;
		//			mciSendString("play ./Music/chomp.mp3 from 0", 0, 0, 0);
		//			plant->HP--;
		//			if (plant->HP == 0)
		//			{
		//				pre = curPlant;
		//				curPlant = curPlant->next;
		//				plants.DeleteNode(pre->content->No);
		//				zombieptr->emerge1walk2eat3 = 2;
		//				continue;
		//			}
		//		}
		//	}
		//	curPlant = curPlant->next;
		//}
		// 如果僵尸前面有除草机
		if (lawnmowers[zombieptr->row]->isOut == 0 && zombieptr->location < lawnmowers[zombieptr->row]->location - 30)
		{
			next = cur->next;
			zombies.DeleteNode(zombieptr->No);
			generateSunshine(zombieptr->location, xys[zombieptr->row][0].y);
			cur = next;
			continue;
		}      
		cur = cur->next;
	}
}

void paintSuns()
{
	Node<Sun> *cur = suns.head, *next;
	while (cur != NULL)
	{
		Sun* sun = cur->content;
		transparentImage(NULL, sun->x, sun->y, &sunPictures[sun->frame]);
		sun->changeFrameCountDown--;
		if (sun->changeFrameCountDown == 0)
		{
			sun->changeFrameCountDown = 5;
			sun->frame++;
			if (sun->frame == 22)sun->frame = 0;
			if (sun->goToCount ==1 )
			{
				sun->x = sun->tempX / 10 * sun->goToCountFrame;
				sun->y = sun->tempY / 10 * sun->goToCountFrame;
				sun->goToCountFrame--;
				if (sun->goToCountFrame == 0)
				{
					next = cur->next;
					suns.DeleteNode(sun->No);
					cur = next;
					currentSunshine += 25;
					continue;
				}
			}
		}
		cur = cur->next;
	}
}

void paintBangs()
{
	Node<Bang>* cur = bangs.head,*pre;
	while (cur != NULL)
	{
		if (cur->content->countDown > 0)
		{
			cur->content->countDown--;
			transparentImage(NULL, cur->content->x, cur->content->y, &bang);
		}
		pre = cur;
		cur = cur->next; 
		if(pre->content->countDown<=0)
			bangs.DeleteNode(pre->content->No);
	}
}

void paintCursor()
{
	if (cursor == Chammer)
	{
		// 如果没锤，画正常角度锤子
		if (!isHitting)
			transparentImage(NULL, mousemsg.x - 45, mousemsg.y - 45, &hammer[0]);
		else
		{
			// 画旋转锤子
			transparentImage(NULL, mousemsg.x - 45, mousemsg.y - 45, &hammer[hammerRadius]);
			hammerRadius++;
			if (hammerRadius == 13)
			{
				hammerRadius = 0;
				isHitting = 0;
			}
		}
	}
	else if (cursor == CpotatoMine)
		transparentImage(NULL, mousemsg.x - 45, mousemsg.y - 45, &potaotoMinePictures[0]);
	else if (cursor == Ciceshroom)
		transparentImage(NULL, mousemsg.x - 45, mousemsg.y - 45, &iceshroomPictures[0]);
	else
		transparentImage(NULL, mousemsg.x - 45, mousemsg.y - 45, &gravebusterPictures[0]);
}

void randomZombies()
{
	// 随机产生僵尸
	for (int i = 0; i < 5; i++)
	{
		for (int j = 3; j < 9; j++)
		{
			if (1 <= mapState[i][j] && mapState[i][j] <= 8)
			{
				double p = rand() / (double)RAND_MAX;
				if (p < normalfrequency)
				{
					NormalZombie* normalZombie = new NormalZombie();
					normalZombie->row = i;
					normalZombie->location = xys[i][j].x-75;
					zombies.InsertNode(normalZombie);
					mciSendString("play ./Music/dirt_rise.mp3 from 0", 0, 0, 0);
				}
				else if (normalfrequency <= p && p < coneheadfrequency)
				{
					ConeheadZombie* coneheadZombie = new ConeheadZombie();
					coneheadZombie->row = i;
					coneheadZombie->location = xys[i][j].x-75;
					zombies.InsertNode(coneheadZombie);
					mciSendString("play ./Music/dirt_rise.mp3 from 0", 0, 0, 0);
				}
				else if (coneheadfrequency <= p && p < bucketheadfrequency)
				{
					BucketheadZombie* bucketheadZombie = new BucketheadZombie();
					bucketheadZombie->row = i;
					bucketheadZombie->location = xys[i][j].x-75;
					zombies.InsertNode(bucketheadZombie);
					mciSendString("play ./Music/dirt_rise.mp3 from 0", 0, 0, 0);
				}
			}
		}
	}

	// 随机呻吟声
	double p = rand() / (double)RAND_MAX;
	if (p < groanFrequency)
	{
		int px = rand() % 6 + 1;
		switch (px)
		{
			case 1:
				mciSendString("play ./Music/groan.mp3 from 0", 0, 0, 0);
				break;
			case 2:
				mciSendString("play ./Music/groan2.mp3 from 0", 0, 0, 0);
				break;
			case 3:
				mciSendString("play ./Music/groan3.mp3 from 0", 0, 0, 0);
				break;
			case 4:
				mciSendString("play ./Music/groan4.mp3 from 0", 0, 0, 0);
				break;
			case 5:
				mciSendString("play ./Music/groan5.mp3 from 0", 0, 0, 0);
				break;
			case 6:
				mciSendString("play ./Music/groan6.mp3 from 0", 0, 0, 0);
				break;
		}
	}
}

void paintNames()
{
	// 画出存档名称
	getFiles("./archives");
	RECT rect;
	setbkmode(TRANSPARENT);
	settextcolor(RGB(222, 186, 97));
	if (files.size() > 5)
	{
		settextstyle(20, 0, "华文隶书");
		rect = { 268, 135, 538, 335 };
		drawtext("存档过多，请删除archives", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rect = { 268, 175, 538, 375 };
		drawtext("文件夹下的存档并重启!", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (files.size() == 0)
	{
		settextstyle(40, 0, "华文隶书");
		rect = { 268, 159, 538, 360 };
		drawtext("没有存档!", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		int h = 189;
		settextstyle(35, 0, "华文隶书");
		for (int i = 0; i < files.size(); ++i)
		{
			rect = { 268, h, 538, h + 40 };
			drawtext(files[i].c_str(), &rect, DT_CENTER);
			h += 40;
		}
	}   
}

void drawHint()
{
	if (drawingHint !=0)
	{
		settextcolor(WHITE);
		settextstyle(40, 0, "隶书");
		if (drawingHint == 1)
		{
			drawtext("此处不能种植物！", &rect, DT_CENTER);
		}
		else if(drawingHint == 2)
			drawtext("阳光不足！", &rect, DT_CENTER);

		hintCountDown--;
		if (hintCountDown == 0)
		{
			hintCountDown = 70;
			drawingHint = 0;
		}
	}
}

void randomGraves()
{
	// 随机产生墓碑
	while (graveNum < 6 || graveNum>13)
	{
		graveNum = 0;
		for (int i = 0; i < 5; i++)
		{
			int num = rand() % 4;
			for (int j = 0; j < num; j++)
			{
				int column = rand() % 6 + 3;
				if (mapState[i][column] == 0)
				{
					mapState[i][column] = rand() % 8 + 1;
					graveNum++;
				}
				else j--;
			}
		}
	}
}

void beginGame()
{
	// 如果是新游戏
	if (isNewGame)
	{
		// 随机产生墓碑
		randomGraves();
		isNewGame = 0;
		cursor = Chammer;
	}
	mciSendString("open ./Music/Loonboon.mp3 alias BGM2", 0, 0, 0);
	mciSendString("play BGM2 repeat", 0, 0, 0);
	mciSendString("play ./Music/theZombiesareComing.mp3 from 0", 0, 0, 0);
	while (1)
	{
		// 绘图
		cleardevice();
		// 画背景、植物条、菜单、阳光数
		putimage(0, 0, &background);
		putimage(0, 0, &plantsBar);
		transparentImage(NULL, 685, 0, &menu);
		RECT r = { 8, 63, 68, 85 };		// 12, 62, 68, 84
		settextstyle(20, 0, "微软雅黑", 0, 0, FW_BOLD, false, false, false);
		settextcolor(BLACK);
		drawtext(itoa(currentSunshine,sunshineNum,10), &r, DT_CENTER);
		// 画植物和墓碑
		paintPlantsAndGraves();
		// 画僵尸
		paintZombies();
		// 画除草机
		for (int i = 0; i < 5; i++)
		{
			if (lawnmowers[i]->isOut == 0)
			{
				transparentImage(NULL, lawnmowers[i]->location, xys[i][0].y + 45, &lawnmower);
			}
		}

		drawHint();		// 画提示

		paintSuns();		// 画太阳

		paintBangs();		// 画 bang

		paintCursor();		// 画鼠标

		// 画雪花
		if (snowCountDown > 0)
		{
			snowCountDown--;
			transparentImage(NULL, 0, 0, &snow);
		}
		FlushBatchDraw();

		// 计算
		// 除草机状态
		for (int i = 0; i < 5; i++)
		{
			if (lawnmowers[i]->isActivated == 1)
			{
				lawnmowers[i]->location += 5;
				if (lawnmowers[i]->location > 800)
				{
					lawnmowers[i]->isOut = 1;
				}
			}
		}

		// 如果点了鼠标
		while (peekmessage(&mousemsg, EM_MOUSE))
		{           
			if (mousemsg.message == WM_LBUTTONDOWN)
			{
				if (mousemsg.x > 692 && mousemsg.y > 0 && mousemsg.x < 815 && mousemsg.y < 44)
				{
					// 如果点击了菜单,存档退出
					writeArchive(username);
					goto stopGame;
				}
				if (cursor == Chammer)
				{
					// 如果鼠标是锤子
					// 如果点了土豆雷
					if (mousemsg.x > 86 && mousemsg.y > 10 && mousemsg.x < 133 && mousemsg.y < 79)
					{
						if (currentSunshine >= 25)
							cursor = CpotatoMine;
						else
							drawingHint = 2;
					}
					// 如果点了墓碑吞噬者
					else if (mousemsg.x > 145 && mousemsg.y > 10 && mousemsg.x < 191 && mousemsg.y < 79)
					{
						if (currentSunshine >= 75)
							cursor = Cgravebuster;
						else
							drawingHint = 2;
					}
					// 如果点了寒冰菇
					else if (mousemsg.x > 204 && mousemsg.y > 10 && mousemsg.x < 253 && mousemsg.y < 79)
					{
						if (currentSunshine >= 75)
							cursor = Ciceshroom;
						else
							drawingHint = 2;
					}
					else
					{
						hammerRadius = 0;
						isHitting = 1;
						mciSendString("play ./Music/hit.mp3 from 0", 0, 0, 0);
						Node<Zombie>* cur = zombies.head;
						while (cur != NULL)
						{
							Zombie* zombie = cur->content;                        
							if (mousemsg.x > zombie->location + 97 && mousemsg.y > xys[zombie->row][0].y - 40
								&& mousemsg.x < zombie->location + 164 && mousemsg.y < xys[zombie->row][0].y + zombie->height)
							{
								// 如果锤到了僵尸，僵尸减血或死亡 
								bangs.InsertNode(new Bang(mousemsg.x - 70, mousemsg.y - 30));
								zombie->HP--;
								if (zombie->HP == 0)
								{
									zombies.DeleteNode(zombie->No);
									generateSunshine(zombie->location, xys[zombie->row][0].y);
								}
								else if (zombie->HP == 1)
									zombie->type = NORMALZOMBIE;        

								goto skipLittleWhile;
							}
							cur = cur->next;
						}
						Node<Sun>* curSun = suns.head;
						while (curSun != NULL)
						{
							Sun* sun = curSun->content;
							if (mousemsg.x > sun->x + 20 && mousemsg.y > sun->y - 18 && mousemsg.x < sun->x + 108 && mousemsg.y < sun->y + 80)
							{
								// 如果锤中太阳
								curSun->content->goToCount = 1;
								mciSendString("play ./Music/sunshine.mp3 from 0", 0, 0, 0);
								goto skipLittleWhile;
							}
							curSun = curSun->next;
						}
					}
				}
				// 如果鼠标是植物
				else
				{
					int i, j, isInPlantZone = 0;
					for (i = 0; i < 5; i++)
					{
						for (j = 0; j < 9; j++)
						{
							if (mousemsg.x > xys[i][j].x && mousemsg.y > xys[i][j].y
								&& mousemsg.x < xys[i][j].x + 80 && mousemsg.y < xys[i][j].y + 100)
							{
								isInPlantZone = 1;
								break;
							}
						}
						if (isInPlantZone)break;
					}

					if ((cursor != Cgravebuster && mapState[i][j] != GRASS) || isInPlantZone == 0)
					{
						drawingHint = 1;
						continue;
					}

					switch (cursor)
					{
						case CpotatoMine:
						{
							currentSunshine -= 25;
							mapState[i][j] = POTATO;
							PotatoMine* potatoMine = new PotatoMine();
							potatoMine->x = i;
							potatoMine->y = j;
							plants.InsertNode(potatoMine);
							mciSendString("play ./Music/plant.mp3 from 0", 0, 0, 0);
							break;
						}
						case Ciceshroom:
						{
							currentSunshine -= 75;
							mapState[i][j] = ICESHROOM;
							IceShroom* iceshroom = new IceShroom();
							iceshroom->x = i;
							iceshroom->y = j;
							plants.InsertNode(iceshroom);
							mciSendString("play ./Music/plant.mp3 from 0", 0, 0, 0);
							break;
						}
						case Cgravebuster:
						{
							if (mapState[i][j] < 1 || mapState[i][j]>8)
							{
								drawingHint = 1;
								continue;
							}
							currentSunshine -= 75;
							switch (mapState[i][j])
							{
								case GRAVE1:
									mapState[i][j] = GRAVEBUSTER_GRAVE1;
									break;
								case GRAVE2:
									mapState[i][j] = GRAVEBUSTER_GRAVE2;
									break;
								case GRAVE3:
									mapState[i][j] = GRAVEBUSTER_GRAVE3;
									break;
								case GRAVE4:
									mapState[i][j] = GRAVEBUSTER_GRAVE4;
									break;
								case GRAVE5:
									mapState[i][j] = GRAVEBUSTER_GRAVE5;
									break;
								case GRAVE6:
									mapState[i][j] = GRAVEBUSTER_GRAVE6;
									break;
								case GRAVE7:
									mapState[i][j] = GRAVEBUSTER_GRAVE7;
									break;
								case GRAVE8:
									mapState[i][j] = GRAVEBUSTER_GRAVE8;
									break;
								default:
									continue;
									break;
							}
							GraveBuster* gravebuster = new GraveBuster();
							gravebuster->x = i;
							gravebuster->y = j;
							plants.InsertNode(gravebuster);
							mciSendString("play ./Music/gravebusterchomp.mp3 from 0", 0, 0, 0);
							break;
						}
					}
					cursor = Chammer;
				}
			}
			else if(mousemsg.message == WM_RBUTTONDOWN)
			{
				cursor = Chammer;
			}
		}

		//判断土豆雷是否被触发
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (mapState[i][j] == POTATOMINE)
				{
					Node<Zombie>* curZombie = zombies.head, * pre = NULL;
					while (curZombie != NULL)
					{                       
						pre = curZombie;
						curZombie = curZombie->next;
						if (pre->content->row == i && pre->content->location>xys[i][j].x - 135 && pre->content->location < xys[i][j].x - 20)
						{
							mapState[i][j] = POTATOBOOM;
							mciSendString("play ./Music/potato_mine.mp3 from 0", 0, 0, 0);
							zombies.DeleteNode(pre->content->No);
						}
					}
				}
			}
		}

		//随机产生僵尸
		randomZombies();

		//判断输赢
		if (graveNum == 0 && zombies.head==NULL)
		{
			Win1Lose2 = 1;
			mciSendString("play ./Music/trophy.mp3 from 0", 0, 0, 0);
			goto stopGame;
		}

		Node<Zombie>* cur = zombies.head;
		while (cur != NULL)
		{
			if (cur->content->location < -150)
			{
				Win1Lose2 = 2;
				mciSendString("play ./Music/losemusic.mp3 from 0", 0, 0, 0);
				goto stopGame;
			}
			cur = cur->next;
		}

	skipLittleWhile:
		//延时
		HpSleep(15);
	}
	stopGame:
	mciSendString("close BGM2", 0, 0, 0);
}

void loadImages(IMAGE imgs[], char path[],int n,int begin)
{
	for (int i = 0; i < n; i++)
	{
		char tmpPath[200], frameNo[4];
		strcpy_s(tmpPath, 200, path);
		strcat(strcat(tmpPath, itoa(i + begin, frameNo, 10)), ".png");
		loadimage(&imgs[i], tmpPath);
	}
}

void loading()
{
	loadImages(grave, "./graphics/GraveStones/", 8, 1);
	loadImages(hammer, "./graphics/Screen/hammer/hammer", 13, 1);
	loadImages(sunPictures, "./graphics/Plants/Sun/Sun_", 22, 0);
	loadImages(potaotoMinePictures, "./graphics/Plants/PotatoMine/PotatoMine/PotatoMine_", 8, 0);
	loadImages(iceshroomPictures, "./graphics/Plants/IceShroom/IceShroom/IceShroom_", 11, 0);
	loadImages(gravebusterPictures, "./graphics/Plants/GraveBuster/GraveBuster-", 28, 1);
	loadImages(normalZombieWalkPictures, "./graphics/Zombies/NormalZombie/Zombie/Zombie-", 47, 1);
	loadImages(coneheadZombieWalkPictures, "./graphics/Zombies/ConeheadZombie/ConeheadZombie/ConeheadZombie-", 47, 1);
	loadImages(bucketheadZombieWalkPictures, "./graphics/Zombies/BucketheadZombie/BucketheadZombie/BucketheadZombie-", 47, 1);
	loadImages(normalZombieEmergePictures, "./graphics/Zombies/NormalZombie/ZombieEmerge/Zombie-", 20, 1);
	loadImages(coneheadZombieEmergePictures, "./graphics/Zombies/ConeheadZombie/ConeheadZombieEmerge/Zombie-", 20, 1);
	loadImages(bucketheadZombieEmergePictures, "./graphics/Zombies/BucketheadZombie/BucketheadZombieEmerge/Zombie-", 20, 1);
	loadImages(normalZombieEatPictures, "./graphics/Zombies/NormalZombie/ZombieAttack/ZombieAttack_", 10, 0);
	loadImages(coneheadZombieEatPictures, "./graphics/Zombies/ConeheadZombie/ConeheadZombieAttack/ConeheadZombieAttack_", 10, 0);
	loadImages(bucketheadZombieEatPictures, "./graphics/Zombies/BucketheadZombie/BucketheadZombieAttack/BucketheadZombieAttack_", 10, 0);

	loadimage(&potatoBoom, "./graphics/Plants/PotatoMine/PotatoMineExplode/PotatoMineExplode_0.png");
	loadimage(&potato, "./graphics/Plants/PotatoMine/PotatoMineInit/PotatoMineInit_0.png");
	loadimage(&plantsBar, "./graphics/Screen/ChooserBackground.png");
	loadimage(&background, "./graphics/Screen/Background.jpg");
	loadimage(&selectID, "./graphics/Screen/selectID.png");
	loadimage(&iceTrap, "./graphics/Plants/IceShroom/IceShroomTrap_0.png");
	loadimage(&snow, "./graphics/Plants/IceShroom/IceShroomSnow_0.png");
	loadimage(&menu, "./graphics/Screen/menu.png");
	loadimage(&lawnmower, "./graphics/Screen/lawnmower.png");
	loadimage(&loseGame, "./graphics/Screen/lose.png");
	loadimage(&winGame, "./graphics/Screen/win.png");
	loadimage(&bang, "./graphics/Screen/bang.png");

	mciSendString("open ./Music/chomp.mp3", 0, 0, 0);
	mciSendString("open ./Music/dirt_rise.mp3", 0, 0, 0);
	mciSendString("open ./Music/gravebusterchomp.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan2.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan3.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan4.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan5.mp3", 0, 0, 0);
	mciSendString("open ./Music/groan6.mp3", 0, 0, 0);
	mciSendString("open ./Music/hit.mp3", 0, 0, 0);
	mciSendString("open ./Music/lawnmower.mp3 alias lawnmower", 0, 0, 0);
	mciSendString("open ./Music/losemusic.mp3", 0, 0, 0);
	mciSendString("open ./Music/plant.mp3", 0, 0, 0);
	mciSendString("open ./Music/potato_mine.mp3", 0, 0, 0);
	mciSendString("open ./Music/shoop.mp3", 0, 0, 0);
	mciSendString("open ./Music/theZombiesareComing.mp3", 0, 0, 0);
	mciSendString("open ./Music/trophy.mp3", 0, 0, 0);
	mciSendString("open ./Music/sunshine.mp3", 0, 0, 0);
}

int main()
{
	srand((unsigned)time(NULL));
	loading();    

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			xys[i][j].x = 40 + j * 82;
			xys[i][j].y = 70 + i * 100;
			//cout << "xys[" << i << "][" << j << "]:" << xys[i][j].x << "," << xys[i][j].y << endl;;
		}
	}
	
	initgraph(820, 600);
	BeginBatchDraw();
	
	labelBGM:
	mciSendString("open ./Music/Cerebrawl.mp3 alias BGM1", 0, 0, 0);
	mciSendString("play BGM1 repeat", 0, 0, 0);

	label2:
	cleardevice();

	//背景
	putimage(0, 0, &background);

	//对话框
	if (Win1Lose2 == 0)
	{
		transparentImage(NULL, 177, 35, &selectID);
		paintNames();
	}
	else if (Win1Lose2 == 1)
	{
		transparentImage(NULL, 230, 140, &winGame);
	}
	else if (Win1Lose2 == 2)
	{
		transparentImage(NULL, 230, 140, &loseGame);
	}

	FlushBatchDraw();
	while (1)
	{
		getmessage(&mousemsg, EM_MOUSE);
		if (mousemsg.message == WM_LBUTTONDOWN)
		{
			cout << mousemsg.x << "," << mousemsg.y << endl;
			if (Win1Lose2 == 0)
			{
				if (mousemsg.x > 236 && mousemsg.y > 436 && mousemsg.x < 391 && mousemsg.y < 474)
				{
					//点击了“没有我的名字”
					char s[10];
					InputBox(s, 10, "请输入你的姓名：");
					init();
					writeArchive(s);
					goto label2;
				}
				else if (mousemsg.x > 410 && mousemsg.y > 438 && mousemsg.x < 566 && mousemsg.y < 473)
				{
					//点击了退出，存档
					if (strcmp(username, "") != 0)
						writeArchive(username);
					return 0;
				}
				else if (mousemsg.x > 268 && mousemsg.y > 190 && mousemsg.x < 538 && mousemsg.y < 385)
				{
					//点击了存档位置
					if (190 <= mousemsg.y && mousemsg.y < 229)
					{
						if (0 < files.size() && files.size() < 6)
							strcpy(username, (char*)files[0].c_str());
						else continue;
					}
					else if (229 <= mousemsg.y && mousemsg.y < 268)
					{
						if (1 < files.size() && files.size() < 6)
							strcpy(username, (char*)files[1].c_str());
						else continue;
					}
					else if (268 <= mousemsg.y && mousemsg.y < 307)
					{
						if (2 < files.size() && files.size() < 6)
							strcpy(username, (char*)files[2].c_str());
						else continue;
					}
					else if (307 <= mousemsg.y && mousemsg.y < 346)
					{
						if (3 < files.size() && files.size() < 6)
							strcpy(username, (char*)files[3].c_str());
						else continue;
					}
					else if (346 <= mousemsg.y && mousemsg.y < 385)
					{
						if (4 < files.size() && files.size() < 6)
							strcpy(username, (char*)files[4].c_str());
						else continue;
					}
					readArchive(username);
					mciSendString("close BGM1", 0, 0, 0);
					beginGame();
					if (Win1Lose2 == 0)
						goto labelBGM;
					else goto label2;
				}
			}
			else
			{
				if (mousemsg.x > 297 && mousemsg.y > 331 && mousemsg.x < 500 && mousemsg.y < 369)
				{
					init();
					mciSendString("close BGM1", 0, 0, 0);
					beginGame();
					if (Win1Lose2 == 0)
						goto labelBGM;
					else goto label2;
				}
			}
		}
	}
	
	getch();
	EndBatchDraw();
	closegraph();

	return 0;
}
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct _Student
{
    char Name[100];
    char gender;
    int age;
}STUDENT, * PSTUDENT;

typedef struct _Node
{
    STUDENT Info;
    struct _Node* Next;
}NODE;

typedef struct _LinkList
{
    NODE* Head;
    NODE* Tail;
    int Num;
}LINKLIST, * PLINKLIST;

bool LinkList_Create(LINKLIST* pLinkList)
{
    pLinkList->Head = (NODE*)malloc(sizeof(NODE));
    if (pLinkList->Head == NULL)
    {
        printf("Not enough memory!\n");
        return false;
    }
    pLinkList->Head->Next = NULL;
    pLinkList->Tail = pLinkList->Head;  // 将Tail指向头结点
    pLinkList->Num = 0;
    return true;
}

NODE* LinkList_CreatNode()
{
    NODE* n;
    n = (NODE*)malloc(sizeof(NODE));
    n->Next = 0;
    return n;
}

bool LinkList_Insert(LINKLIST* pLinkList, NODE** pNew, int Pos)
{
    NODE* p;
    int j;

    if (Pos == -1)  // 插入到尾部
    {
        (*pNew)->Next = NULL;
        pLinkList->Tail->Next = *pNew;
        pLinkList->Tail = *pNew;
        pLinkList->Num++;
        return true;
    }

    p = pLinkList->Head;

    for (j = 0; j < Pos && p != NULL; j++)
        p = p->Next;

    if (p == NULL)
    {
        printf("插入失败\n");
        return false;
    }

    (*pNew)->Next = p->Next;
    p->Next = *pNew;
    if (p->Next == NULL)  // 更新尾节点
    {
        pLinkList->Tail = *pNew;
    }
    pLinkList->Num++;

    return true;
}

bool LinkList_Delete(LINKLIST* pLinkList, int Pos)
{
    NODE* p, * q;
    int j;
    if (Pos == 0)
        return false;
    p = pLinkList->Head;
    for (j = 1; j < Pos && p->Next != NULL; j++)
        p = p->Next;
    if (p->Next == NULL)
    {
        printf("the %d node not foundt\n", Pos);
        return false;
    }
    q = p->Next;
    p->Next = q->Next;
    if (p->Next == NULL)  // 更新尾节点
    {
        pLinkList->Tail = p;
    }
    free(q);
    pLinkList->Num--;
    return true;
}

bool linklist_print(LINKLIST* linklist)
{
    NODE* p;
    int i = 1;
    p = linklist->Head;
    if (p->Next == NULL)
    {
        printf("空表");
        return false;
    }
    p = p->Next;
    while (p != NULL)
    {
        printf("%5d  姓名 %s  性别 %c  年龄 %d\n", i, p->Info.Name, p->Info.gender, p->Info.age);
        p = p->Next;
        i++;
    }
    return true;
}

int main()
{
    int operate;
    printf("请选择你的操作：\n1.创建新的学生信息\n2.加入学生信息\n3.删除学生信息\n4.输出学生信息\n5.结束操作\n");
    scanf("%d", &operate);
    LINKLIST ll;
    ll.Head = NULL;
    ll.Num = 0;
    LinkList_Create(&ll);  // 创建链表
    while (operate)
    {

        switch (operate)
        {
        case 1:
        {
            NODE* n1 = LinkList_CreatNode();
            char s[100], G;
            int age;
            printf("请依次输入姓名：");
            scanf("%s", &s);
            strcpy(n1->Info.Name, s);
            rewind(stdin);
            printf("请依次输入性别（M|W）：");
            scanf("%c", &G);
            n1->Info.gender = G;
            rewind(stdin);
            printf("请依次输入年龄：");
            scanf("%d", &age);
            n1->Info.age = age;

            LinkList_Insert(&ll, &n1, -1);  // 插入到尾部

            break;
        }
        case 2:
        {
            int j;
            printf("选择加入的位置：");
            scanf("%d", &j);
            if (j >= 1)
            {
                printf("输入错误,请输入0或 - 1\n");
                break;
            }
            NODE* n2 = LinkList_CreatNode();
            char s[100], G;
            int age;
            printf("请依次输入姓名：");
            scanf("%s", &s);
            strcpy(n2->Info.Name, s);
            rewind(stdin);
            printf("请依次输入性别（M|W）：");
            scanf("%c", &G);
            n2->Info.gender = G;
            rewind(stdin);
            printf("请依次输入年龄：");
            scanf("%d", &age);
            n2->Info.age = age;

            LinkList_Insert(&ll, &n2, j);  // 插入到指定位置

            break;
        }
        case 3:
        {
            int k;
            printf("选择删除的位置：");
            scanf("%d", &k);
            LinkList_Delete(&ll, k);  // 删除指定位置的节点
            break;
        }
        case 4:
        {
            linklist_print(&ll);  // 输出链表中的数据
            break;
        }
        default:
        {
            return 1;
        }
        }
        printf("请选择你的操作：\n1.创建新的学生信息\n2.加入学生信息\n3.删除学生信息\n4.输出学生信息\n5.结束操作\n");
        scanf("%d", &operate);
    }

    return 0;
}
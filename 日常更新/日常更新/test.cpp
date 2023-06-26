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
    pLinkList->Tail = pLinkList->Head;  // ��Tailָ��ͷ���
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

    if (Pos == -1)  // ���뵽β��
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
        printf("����ʧ��\n");
        return false;
    }

    (*pNew)->Next = p->Next;
    p->Next = *pNew;
    if (p->Next == NULL)  // ����β�ڵ�
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
    if (p->Next == NULL)  // ����β�ڵ�
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
        printf("�ձ�");
        return false;
    }
    p = p->Next;
    while (p != NULL)
    {
        printf("%5d  ���� %s  �Ա� %c  ���� %d\n", i, p->Info.Name, p->Info.gender, p->Info.age);
        p = p->Next;
        i++;
    }
    return true;
}

int main()
{
    int operate;
    printf("��ѡ����Ĳ�����\n1.�����µ�ѧ����Ϣ\n2.����ѧ����Ϣ\n3.ɾ��ѧ����Ϣ\n4.���ѧ����Ϣ\n5.��������\n");
    scanf("%d", &operate);
    LINKLIST ll;
    ll.Head = NULL;
    ll.Num = 0;
    LinkList_Create(&ll);  // ��������
    while (operate)
    {

        switch (operate)
        {
        case 1:
        {
            NODE* n1 = LinkList_CreatNode();
            char s[100], G;
            int age;
            printf("����������������");
            scanf("%s", &s);
            strcpy(n1->Info.Name, s);
            rewind(stdin);
            printf("�����������Ա�M|W����");
            scanf("%c", &G);
            n1->Info.gender = G;
            rewind(stdin);
            printf("�������������䣺");
            scanf("%d", &age);
            n1->Info.age = age;

            LinkList_Insert(&ll, &n1, -1);  // ���뵽β��

            break;
        }
        case 2:
        {
            int j;
            printf("ѡ������λ�ã�");
            scanf("%d", &j);
            if (j >= 1)
            {
                printf("�������,������0�� - 1\n");
                break;
            }
            NODE* n2 = LinkList_CreatNode();
            char s[100], G;
            int age;
            printf("����������������");
            scanf("%s", &s);
            strcpy(n2->Info.Name, s);
            rewind(stdin);
            printf("�����������Ա�M|W����");
            scanf("%c", &G);
            n2->Info.gender = G;
            rewind(stdin);
            printf("�������������䣺");
            scanf("%d", &age);
            n2->Info.age = age;

            LinkList_Insert(&ll, &n2, j);  // ���뵽ָ��λ��

            break;
        }
        case 3:
        {
            int k;
            printf("ѡ��ɾ����λ�ã�");
            scanf("%d", &k);
            LinkList_Delete(&ll, k);  // ɾ��ָ��λ�õĽڵ�
            break;
        }
        case 4:
        {
            linklist_print(&ll);  // ��������е�����
            break;
        }
        default:
        {
            return 1;
        }
        }
        printf("��ѡ����Ĳ�����\n1.�����µ�ѧ����Ϣ\n2.����ѧ����Ϣ\n3.ɾ��ѧ����Ϣ\n4.���ѧ����Ϣ\n5.��������\n");
        scanf("%d", &operate);
    }

    return 0;
}
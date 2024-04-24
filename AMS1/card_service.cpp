#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>    
#include <string.h>     //包含字符处理头文件
#include <time.h>
#include <stdio.h>

#include "model.h"      //包含数据定义头文件
#include "global.h"     //包含全局定义头文件
#include "card_file.h"
#include "billing_service.h"
#include "tool.h"

lpCardNode cardList = NULL;    //定义卡信息链表
int getCard();                 //声明getCard函数
//初始化链表
int initCardList()
{
	lpCardNode head = NULL;
	if (cardList == NULL)
	{
		//为链表头结点分配内存
		head = (lpCardNode)malloc(sizeof(CardNode));  //head是lpCardNode类型，而malloc函数返回的是void*类型，需要强制转换。

		//如果分配成功，则将链表指向
		if (head != NULL)
		{
			head->next = NULL;
			cardList = head;
			return TRUE;
		}

	}
	return FALSE;
}

//释放结点
void releaseCardList()
{
	lpCardNode cur = cardList;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next;   //释放cur结点前，用next保存它的后续结点
		free(cur);         //释放cur结点
		cur = next;
	}
	cardList = NULL;
}

int addCard(Card card)
{
	//将卡信息保存到文件中
	if (TRUE == saveCard(&card, CARDPATH))
	{
		return TRUE;
	}
	return FALSE;

}

Card* queryCard(const char* pName)
{

	Card* pCard = NULL;
	lpCardNode node = NULL;

	//从卡信息文件中读取卡信息到链表，失败返回NULL
	if (FALSE == getCard())
	{
		return NULL;
	}
	//指向链表的第一个结点
	node = cardList->next;
	//在链表中查找指定卡号
	while (node != NULL)
	{
		//如果链表结点中有卡号相同的信息，则返回结点的数据
		if (strcmp(node->data.aName, pName) == 0)
		{
			pCard = &node->data;
			return pCard;
		}

		//移到链表的下一个结点
		node = node->next;
	}

	return pCard;
}

Card* queryCards(const char* pName, int* pIndex)
{
	lpCardNode node = NULL;
	Card* pCard = NULL;
	if (FALSE == getCard())
	{
		return NULL;
	}

	//首先分配一个Card大小的内存空间
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
	{
		return NULL;
	}

	//从链表的头结点指向的下一个结点开始遍历
	node = cardList->next;

	//遍历链表，结点为空表示到达链表尾部
	while (node != NULL)
	{
		//判断在遍历到的结点的学号中，查找是否包含pName的字符串
		if (strstr(node->data.aName, pName) != NULL)
		{
			//如果有，则保存结点中的数据
			pCard[*pIndex] = node->data;
			(*pIndex)++;

			//重新为指针分配内存
			pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
		}
		//移到链表的下一个结点
		node = node->next;
	}
	return pCard;
}

Card* queryAll(int* pIndex)
{
	lpCardNode node = NULL;
	Card* pCard = NULL;
	if (FALSE == getCard())
	{
		return NULL;
	}

	//首先分配一个Card大小的内存空间
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
	{
		return NULL;
	}

	//从链表的头结点指向的下一个结点开始遍历
	node = cardList->next;

	//遍历链表，结点为空表示到达链表尾部
	while (node != NULL)
	{
		pCard[*pIndex] = node->data;
		(*pIndex)++;

		//重新为指针分配内存
		pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));

		//移到链表的下一个结点
		node = node->next;
	}
	return pCard;
}

//将文件中的卡信息保存到链表中
int getCard()
{
	int nCount = 0;
	int i = 0;
	Card* pCard = NULL;
	lpCardNode node = NULL;
	lpCardNode cur = NULL;      //定义结点
	if (cardList != NULL)
	{
		releaseCardList();
	}

	//初始化链表
	initCardList();

	//获取卡信息数量

	nCount = getCardCount(CARDPATH);

	if (nCount == 0)
	{
		return FALSE;
	}

	//动态分配内存保存卡信息 
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		return FALSE;
	}

	//获取卡信息
	if (FALSE == readCard(pCard, CARDPATH))
	{
		free(pCard);
		pCard = NULL;
		return FALSE;
	}
	for (i = 0, node = cardList; i < nCount; i++)
	{
		//为结点分配内存
		cur = (lpCardNode)malloc(sizeof(CardNode));

		//如果分配内存失败，则返回
		if (cur == NULL)
		{
			free(pCard);
			return FALSE;
		}
		//初始化新的空间，全部赋值为0
		memset(cur, 0, sizeof(CardNode));

		//将卡信息保存到结点中
		cur->data = pCard[i];
		cur->next = NULL;

		//将结点添加到链表尾部
		node->next = cur;
		node = cur;

 	}
	free(pCard);
	pCard = NULL;
	return TRUE;

}

Card* checkCard(const char* pName, const char* pPwd, int* pIndex)
{
	lpCardNode cardNode = NULL;
	int nIndex = 0;

	//获取文件中的卡信息,如果失败，则上机失败
	if (FALSE == getCard())
	{
		return FALSE;
	}

	cardNode = cardList->next;
	//遍历链表，判断能否上机
	while (cardNode != NULL)
	{
		//判断卡号和密码是否正确
		if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
		{
			//返回卡信息结点数据的地址
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return NULL;
}

int queryDates(const char* pName)
{
	char time[20] = { 0 };
	int count = 0;
	lpCardNode node = NULL;
	if (FALSE == getCard())
	{
		return NULL;
	}

	//从链表的头结点指向的下一个结点开始遍历
	node = cardList->next;

	//遍历链表，结点为空表示到达链表尾部
	while (node != NULL)
	{
		timeToString(node->data.tLastUse, time);
		//判断在遍历到的结点的学号中，查找是否包含pName的字符串
		if (strstr(time, pName) != NULL)
		{
			//如果有，则count++
			count++;
		}
		//移到链表的下一个结点
		node = node->next;
	}
	return count;
}

float queryCardtime(char* begintime, char* endtime)
{
	char time[7] = {" 00:00"};
	strcat(begintime, time);
	strcat(endtime, time);
	float amount = 0;
	int count = 0;
	time_t begin = stringToTime(begintime);
	time_t end = stringToTime(endtime);
	lpCardNode node = NULL;
	if (FALSE == getCard())
	{
		return NULL;
	}

	//从链表的头结点指向的下一个结点开始遍历
	node = cardList->next;

	//遍历链表，结点为空表示到达链表尾部
	while (node != NULL)
	{
		//判断在遍历到的结点的卡号中，查找是否包含符合时间范围的卡
		if (node->data.tLastUse >= begin && node->data.tLastUse <= end)
		{
			amount = amount + node->data.fTotalUse;
			//如果有，则加入该卡的消费额
			
		}
		//移到链表的下一个结点
		node = node->next;
	}
	return amount;
}
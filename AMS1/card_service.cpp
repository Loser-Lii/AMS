#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>    
#include <string.h>     //�����ַ�����ͷ�ļ�
#include <time.h>
#include <stdio.h>

#include "model.h"      //�������ݶ���ͷ�ļ�
#include "global.h"     //����ȫ�ֶ���ͷ�ļ�
#include "card_file.h"
#include "billing_service.h"
#include "tool.h"

lpCardNode cardList = NULL;    //���忨��Ϣ����
int getCard();                 //����getCard����
//��ʼ������
int initCardList()
{
	lpCardNode head = NULL;
	if (cardList == NULL)
	{
		//Ϊ����ͷ�������ڴ�
		head = (lpCardNode)malloc(sizeof(CardNode));  //head��lpCardNode���ͣ���malloc�������ص���void*���ͣ���Ҫǿ��ת����

		//�������ɹ���������ָ��
		if (head != NULL)
		{
			head->next = NULL;
			cardList = head;
			return TRUE;
		}

	}
	return FALSE;
}

//�ͷŽ��
void releaseCardList()
{
	lpCardNode cur = cardList;
	lpCardNode next = NULL;

	while (cur != NULL)
	{
		next = cur->next;   //�ͷ�cur���ǰ����next�������ĺ������
		free(cur);         //�ͷ�cur���
		cur = next;
	}
	cardList = NULL;
}

int addCard(Card card)
{
	//������Ϣ���浽�ļ���
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

	//�ӿ���Ϣ�ļ��ж�ȡ����Ϣ������ʧ�ܷ���NULL
	if (FALSE == getCard())
	{
		return NULL;
	}
	//ָ������ĵ�һ�����
	node = cardList->next;
	//�������в���ָ������
	while (node != NULL)
	{
		//������������п�����ͬ����Ϣ���򷵻ؽ�������
		if (strcmp(node->data.aName, pName) == 0)
		{
			pCard = &node->data;
			return pCard;
		}

		//�Ƶ��������һ�����
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

	//���ȷ���һ��Card��С���ڴ�ռ�
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
	{
		return NULL;
	}

	//�������ͷ���ָ�����һ����㿪ʼ����
	node = cardList->next;

	//�����������Ϊ�ձ�ʾ��������β��
	while (node != NULL)
	{
		//�ж��ڱ������Ľ���ѧ���У������Ƿ����pName���ַ���
		if (strstr(node->data.aName, pName) != NULL)
		{
			//����У��򱣴����е�����
			pCard[*pIndex] = node->data;
			(*pIndex)++;

			//����Ϊָ������ڴ�
			pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
		}
		//�Ƶ��������һ�����
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

	//���ȷ���һ��Card��С���ڴ�ռ�
	pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
	{
		return NULL;
	}

	//�������ͷ���ָ�����һ����㿪ʼ����
	node = cardList->next;

	//�����������Ϊ�ձ�ʾ��������β��
	while (node != NULL)
	{
		pCard[*pIndex] = node->data;
		(*pIndex)++;

		//����Ϊָ������ڴ�
		pCard = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));

		//�Ƶ��������һ�����
		node = node->next;
	}
	return pCard;
}

//���ļ��еĿ���Ϣ���浽������
int getCard()
{
	int nCount = 0;
	int i = 0;
	Card* pCard = NULL;
	lpCardNode node = NULL;
	lpCardNode cur = NULL;      //������
	if (cardList != NULL)
	{
		releaseCardList();
	}

	//��ʼ������
	initCardList();

	//��ȡ����Ϣ����

	nCount = getCardCount(CARDPATH);

	if (nCount == 0)
	{
		return FALSE;
	}

	//��̬�����ڴ汣�濨��Ϣ 
	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
	{
		return FALSE;
	}

	//��ȡ����Ϣ
	if (FALSE == readCard(pCard, CARDPATH))
	{
		free(pCard);
		pCard = NULL;
		return FALSE;
	}
	for (i = 0, node = cardList; i < nCount; i++)
	{
		//Ϊ�������ڴ�
		cur = (lpCardNode)malloc(sizeof(CardNode));

		//��������ڴ�ʧ�ܣ��򷵻�
		if (cur == NULL)
		{
			free(pCard);
			return FALSE;
		}
		//��ʼ���µĿռ䣬ȫ����ֵΪ0
		memset(cur, 0, sizeof(CardNode));

		//������Ϣ���浽�����
		cur->data = pCard[i];
		cur->next = NULL;

		//�������ӵ�����β��
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

	//��ȡ�ļ��еĿ���Ϣ,���ʧ�ܣ����ϻ�ʧ��
	if (FALSE == getCard())
	{
		return FALSE;
	}

	cardNode = cardList->next;
	//���������ж��ܷ��ϻ�
	while (cardNode != NULL)
	{
		//�жϿ��ź������Ƿ���ȷ
		if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd) == 0))
		{
			//���ؿ���Ϣ������ݵĵ�ַ
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

	//�������ͷ���ָ�����һ����㿪ʼ����
	node = cardList->next;

	//�����������Ϊ�ձ�ʾ��������β��
	while (node != NULL)
	{
		timeToString(node->data.tLastUse, time);
		//�ж��ڱ������Ľ���ѧ���У������Ƿ����pName���ַ���
		if (strstr(time, pName) != NULL)
		{
			//����У���count++
			count++;
		}
		//�Ƶ��������һ�����
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

	//�������ͷ���ָ�����һ����㿪ʼ����
	node = cardList->next;

	//�����������Ϊ�ձ�ʾ��������β��
	while (node != NULL)
	{
		//�ж��ڱ������Ľ��Ŀ����У������Ƿ��������ʱ�䷶Χ�Ŀ�
		if (node->data.tLastUse >= begin && node->data.tLastUse <= end)
		{
			amount = amount + node->data.fTotalUse;
			//����У������ÿ������Ѷ�
			
		}
		//�Ƶ��������һ�����
		node = node->next;
	}
	return amount;
}
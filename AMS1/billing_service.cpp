#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "model.h"
#include "global.h"
#include "billing_file.h"


lpBillingNode billingList = NULL;

void initBillingList()
{
	lpBillingNode cur = NULL;
	if (billingList == NULL)
	{
		cur = (lpBillingNode)malloc(sizeof(BillingNode));
		cur->next = NULL;
		billingList = cur;
	}
}
int addBilling(const char* pName, Billing* pBilling)
{
	//������Ϣд��ṹ��
	strcpy(pBilling->aCardName, pName);   //�ϻ�����
	pBilling->tStart = time(NULL);        //�ϻ�ʱ��
	pBilling->tEnd = 0;                   //�»�ʱ��
	pBilling->fAmount = 0;                //���ѽ��
	pBilling->nStatus = 0;                //����״̬��0-δ���㣬1-�ѽ���
	pBilling->nDel = 0;                   //ɾ����ʶ��0-δɾ����1-��ɾ��

	//������Ϣ�ṹ��д���ļ�
	if (FALSE == saveBilling(pBilling, BILLINGPATH))
	{
		return FALSE;
	}
	return TRUE;
}

void releaseBillingList()
{
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;

	//��������
	while (cur != NULL)
	{
		next = cur->next;   //����ڴ��ͷ�ǰ��next��������
		free(cur);    //�ͷŽ���ڴ�
		cur = next;
	}
	billingList = NULL;
}

int getBilling()
{
	int nCount = 0;       //�Ʒ���Ϣ����
	Billing* pBilling = NULL;     //�Ʒ���Ϣ
	lpBillingNode node = NULL;
	int i = 0;        //ѭ������
	lpBillingNode cur = NULL;

	//�������Ϊ�գ��ͷ�����
	if (billingList != NULL)
	{
		releaseBillingList();
	}

	//��ʼ������
	initBillingList();

	//��ȡ�Ʒ���Ϣ����
	nCount = getBillingCount(BILLINGPATH);
	//��̬�����ڴ�
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL)
	{
		//��ȡ�Ʒ���Ϣ
		if (FALSE == readBilling(pBilling, BILLINGPATH))
		{
			free(pBilling);
			return FALSE;
		}

		//���Ʒ���Ϣ���浽������
		for (i = 0, node = billingList; i < nCount; i++)
		{
			//Ϊ��ǰ�������ڴ�
			cur = (lpBillingNode)malloc(sizeof(BillingNode));
			//�������ʧ�ܣ����ڷ���FALSE֮ǰ����Ҫ�ͷ�pBilling���ڴ�
			if (cur == NULL)
			{
				free(pBilling);
				return FALSE;
			}
			//��ʼ���µĿռ䣬ȫ����ֵΪ0��
			memset(cur, 0, sizeof(BillingNode));
			//��������ӵ������
			cur->data = pBilling[i];
			cur->next = NULL;
			//�������ӵ�������
			node->next = cur;
			node = cur;
		}
		free(pBilling);
		return TRUE;
	}
	return FALSE;
}

Billing* queryBilling(const char* pName, int* pIndex)
{
	lpBillingNode node = NULL;
	int nIndex = 0;

	if (FALSE == getBilling())
	{
		return NULL;
	}
	//��������
	node = billingList->next;
	while (node != NULL)
	{
		//��ѯ��������ͬ������û�н���ļƷ���Ϣ
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0)
		{
			return &node->data;
		}
		node = node->next;
		nIndex++;
		*pIndex = nIndex;
	}
	return NULL;
}

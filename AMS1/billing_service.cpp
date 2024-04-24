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
	//消费信息写入结构体
	strcpy(pBilling->aCardName, pName);   //上机卡号
	pBilling->tStart = time(NULL);        //上机时间
	pBilling->tEnd = 0;                   //下机时间
	pBilling->fAmount = 0;                //消费金额
	pBilling->nStatus = 0;                //消费状态，0-未结算，1-已结算
	pBilling->nDel = 0;                   //删除标识，0-未删除，1-已删除

	//消费信息结构体写入文件
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

	//销毁链表
	while (cur != NULL)
	{
		next = cur->next;   //结点内存释放前，next保存其后继
		free(cur);    //释放结点内存
		cur = next;
	}
	billingList = NULL;
}

int getBilling()
{
	int nCount = 0;       //计费信息数量
	Billing* pBilling = NULL;     //计费信息
	lpBillingNode node = NULL;
	int i = 0;        //循环变量
	lpBillingNode cur = NULL;

	//如果链表不为空，释放链表
	if (billingList != NULL)
	{
		releaseBillingList();
	}

	//初始化链表
	initBillingList();

	//获取计费信息数量
	nCount = getBillingCount(BILLINGPATH);
	//动态分配内存
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL)
	{
		//获取计费信息
		if (FALSE == readBilling(pBilling, BILLINGPATH))
		{
			free(pBilling);
			return FALSE;
		}

		//将计费信息保存到链表中
		for (i = 0, node = billingList; i < nCount; i++)
		{
			//为当前结点分配内存
			cur = (lpBillingNode)malloc(sizeof(BillingNode));
			//如果分配失败，则在返回FALSE之前，需要释放pBilling的内存
			if (cur == NULL)
			{
				free(pBilling);
				return FALSE;
			}
			//初始化新的空间，全部赋值为0；
			memset(cur, 0, sizeof(BillingNode));
			//将数据添加到结点中
			cur->data = pBilling[i];
			cur->next = NULL;
			//将结点添加到链表中
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
	//遍历链表
	node = billingList->next;
	while (node != NULL)
	{
		//查询到卡号相同，并且没有结算的计费信息
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

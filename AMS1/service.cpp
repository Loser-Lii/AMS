#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "tool.h"
#include "model.h"
#include "global.h"
#include "card_service.h"
#include "card_file.h"
#include "billing_service.h"	
#include "billing_file.h"
#include "statistics.h"

double getAmount(time_t tStart);    //����getAmount()����
//��ӿ�
int addCardInfo(Card card)
{
	return addCard(card);

}
//��������
Card* queryCardInfo(const char* pName) 
{

	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;

}
//��ѯ��
Card* queryCardsInfo(const char* pName, int* pIndex)
{
	Card* pCard = NULL;
	pCard = queryCards(pName, pIndex);
	return pCard;
}
int initCardList();

//�ϻ�
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;     //����Ϣ�������е�����
	Billing billing;    //�Ʒ���Ϣ

	//���ݿ��ź����룬�������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);

	//�������ϢΪ�գ���ʾû�иÿ���Ϣ���ϻ�ʧ��
	if (pCard == NULL)
	{
		printf("--**���Ż����������������������**--\n");
		return FALSE;
	}
	//�����״̬��Ϊ0����ʾ�ÿ������ϻ�
	if (pCard->nStatus == 1)
	{
		return UNUSE;
	}
	if (pCard->nStatus == 2 )
	{
		return ANNULED;
	}
	//��������Ϊ0�������ϻ�
	if (pCard->fBalance <= 0)
	{
		return ENOUGHMONEY;
	}

	//��������ϻ������¿���Ϣ
	pCard->nStatus = 1;       //״̬Ϊ����ʹ��
	pCard->nUseCount++;      //ʹ�ô���+1
	pCard->tStart = time(NULL);
	pCard->tLastUse = time(NULL);

	//�����ļ��еĿ���Ϣ
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}

	//������Ѽ�¼
	strcpy(billing.aCardName, pName);      //�ϻ�����
	billing.tStart = time(NULL);           //�ϻ�ʱ��
	billing.tEnd = 0;                      //�»�ʱ��
	billing.nStatus = 0;                   //����״̬
	billing.fAmount = 0;                   //���ѽ��
	billing.nDel = 0;                      //ɾ����ʶ

	//���Ʒ���Ϣ���浽�ļ���
	if (TRUE == saveBilling(&billing, BILLINGPATH))
	{
		//��װ�ϻ���Ϣ
		strcpy(pInfo->aCardName, pName);
		pInfo->fBalance = pCard->fBalance;
		pInfo->tLogon = billing.tStart;
		return TRUE;
	}

	return FALSE;
}

int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo)
{
	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int nIndex = 0;               //����Ϣ�������е�������
	int nPosition = 0;            //�Ʒ���Ϣ�������е�������
	double dbAmount = 0.0;        //���ѽ��
	float fBalance = 0.0;         //���

	//��ѯ�ϻ���
	pCard = checkCard(pName, pPwd, &nIndex);

	//�������ϢΪ�գ���ʾû�иÿ���Ϣ������FALSE
	if (pCard == NULL)
	{
		printf("--**���Ż����������������������**--\n");
		return FALSE;
	}

	//�жϿ��Ƿ����ϻ���ֻ�������ϻ��Ŀ����ܽ����»�����
	if (pCard->nStatus == 0)
	{
		return UNUSE;
	}

	if (pCard->nStatus == 2)
	{
		return ANNULED;
	}

	//���ݿ��ţ���ѯ�Ʒ���Ϣ
	pBilling = queryBilling(pName, &nPosition);

	//�����ѯ�Ʒ���ϢΪ�գ���ʾ�»�ʧ��
	if (pBilling == NULL)
	{
		printf("--**���Ż����������������������**--\n");
		return FALSE;
	}

	//�������ѽ��
	dbAmount = getAmount(pCard->tStart);

	//������С�����ѽ����ܽ����»�
	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance < 0)
	{
		return ENOUGHMONEY;
	}
	
	//��װ�»���Ϣ
	strcpy(pInfo->aCardName, pName);           //�ϻ�����
	pInfo->fAmount = (float)dbAmount;                 //���ѽ��
	pInfo->fBalance = fBalance;                //���
	pInfo->tStart = pBilling->tStart;          //�ϻ�ʱ��
	pInfo->tEnd = time(NULL);                  //�»�ʱ��

	//���¿���Ϣ
	pCard->fTotalUse = pCard->fTotalUse + pInfo->fAmount;  //���������п���Ϣ�ۼ�ʹ�ý��
	pCard->fBalance = fBalance;   //���������п���Ϣ�����
	pCard->nStatus = 0;           //״̬
	pCard->tLastUse = time(NULL); //�ϴ�ʹ��ʱ��

	//�����ļ��еĿ���Ϣ
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}

	//����������Ϣ
	pBilling->fAmount = (float)dbAmount;    //������Ϣ
	pBilling->nStatus = 1;                  //״̬-�ѽ���
	pBilling->tEnd = time(NULL);            //�»�ʱ��

	//�����ļ��еĿ���Ϣ
	if (FALSE == updateBilling(pBilling, BILLINGPATH, nPosition))
	{
		return FALSE;
	}

	return TRUE;
}

double getAmount(time_t  tStart)
{
	int	nSec = 0;               //����ʱ�䣨�룩
	int nMin = 0;               //����ʱ�䣨���ӣ�
	int nCount = 0;             //�ϻ���ʱ�䵥Ԫ����ÿ����Ԫ15����
	double dbAmount = 0;        //���ѽ��
	time_t tEnd = time(NULL);   //����ʱ��Ϊ��ǰʱ��

	//��������ʱ��
	nSec = (int)(tEnd - tStart);  //����
	if (nSec % 60 == 0)
	{
		nMin = nSec / 60;    //����
	}
	else
	{
		nMin = nSec / 60 + 1; //������
	}

	//�������ѵ�ʱ�䵥Ԫ��
	if (nMin % UNIT == 0)
	{
		nCount = nMin / UNIT;    //����
	}
	else
	{
		nCount = nMin / UNIT + 1; //������
	}

	//�������ѽ��
	dbAmount = nCount * CHARGE;

	return dbAmount;
}

void releaseList()
{
	releaseCardList();            //�ͷſ���Ϣ�����ڴ�
	releaseBillingList();         //�ͷżƷ���Ϣ�����ڴ�
}
	
Card* doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//���ҿ���Ϣ
	pCard= checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	}
	else if(strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{   
		//״̬ȷ��,	�ж��ܷ��ֵ
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---�ÿ���ע��---***----\n---**--�������ʹ�������¼���--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---�ÿ��ѹ���---***----\n---**--����Ҫ�˿���ֱ��ע����--**---\n   --*-�������ʹ�������¼���-*--\n");
			return NULL;
		}
		//���¿���Ϣ
		pCard->fBalance = pCard->fBalance + pMoneyInfo->fMoney;
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH,nIndex) == TRUE)
			return pCard;
	}
	
}

Card* doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//���ҿ���Ϣ
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//״̬ȷ��,	�ж��ܷ��˷�
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---�ÿ���ע��---***----\n---**--�������ʹ�������¼���--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---�ÿ��ѹ���---***----\n---**--����Ҫ�˿���ֱ��ע����--**---\n   --*-�������ʹ�������¼���-*--\n");
			return NULL;
		}
		//���¿���Ϣ
		if(pMoneyInfo->fMoney<= pCard->fBalance)
		{
			pCard->fBalance = pCard->fBalance - pMoneyInfo->fMoney;
		}
		else
		{
			printf("\n--**���������˷�**--\n");
			return NULL;
		}
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}
Card* doAnnul(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//��ѯ����Ϣ
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	} 
	else if(strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//״̬ȷ��,	�ж��ܷ�ע��
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---�ÿ���ע��---***----\n---**--�������ʹ�������¼���--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---�ÿ��ѹ���---***----\n---**--����Ҫ�˿���ֱ��ע����--**---\n   --*-�������ʹ�������¼���-*--\n");
			return NULL;
		}
		//���¿���Ϣ:�����ʹ��ʱ�䡢״̬��ɾ����ʶ
		pCard->fBalance = 0;
		pCard->nStatus = 2;
		pCard->nDel = 1;
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doRenew(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//��ѯ����Ϣ
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//״̬ȷ��,	�ж��ܷ��ֵ
		if (pCard->nStatus != 2)
		{
			printf("\n----***---�ÿ�δע��---***----\n");
			return NULL;
		}
		//���¿���Ϣ:�����ʹ��ʱ�䡢״̬��ɾ����ʶ
		pCard->nStatus = 0;
		pCard->nDel = 0;
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doChange(const char* pName,const char* pPwd,const char* cPwd) 
{
	Card* pCard = NULL;
	int nIndex = 0;
	//��ѯ����Ϣ
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//״̬ȷ��,	�ж��ܷ��޸�����
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---�ÿ���ע��---***----\n---**--�������ʹ�������¼���--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---�ÿ��ѹ���---***----\n---**--����Ҫ�˿���ֱ��ע����--**---\n   --*-�������ʹ�������¼���-*--\n");
			return NULL;
		}
		strcpy(pCard->aPwd, cPwd);
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doVip(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//��ѯ����Ϣ
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**���Ż������������**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
	//״̬ȷ��,	�ж��ܷ�ע���Ա
		if (pCard->nVip == 1)
		{
			printf("\n----***---�ÿ����ǻ�Ա---***----\n");
			return NULL;
		}
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---�ÿ���ע��---***----\n---**--�������ʹ�������¼���--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---�ÿ��ѹ���---***----\n---**--����Ҫ�˿���ֱ��ע����--**---\n   --*-�������ʹ�������¼���-*--\n");
			return NULL;
		}
		pCard->nVip = 1;
		//���¿��ļ���Ϣ
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
		}
}

//��ӡ��ά�뺯��
int printQR()
{
	char c;
	FILE* fp;
	fp = fopen($QR$, "r");
	if (fp == NULL)
	{
		printf("File is not found\n");
		return 0;
	}
	
	while ((c = fgetc(fp)) != EOF)
	{
		putchar(c);
	}
	
	printf("�밴Ctrl+�����ֽ������ţ����������ʴ�С\n");
	printf("��ֵ��ɺ����������\n");
	getchar();
	fclose(fp);
	return 1;
}

void showcard(Card* pCard)
{
	char aLastTime[50];
	printf("����\t\t״̬\t\t���\t\t�ۼ�ʹ��\t\tʹ�ô���\t\t�ϴ�ʹ��ʱ��\n");
	timeToString(pCard->tLastUse, aLastTime);
	printf("%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", pCard->aName, pCard->nStatus,
		pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, aLastTime);
}
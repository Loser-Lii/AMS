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

double getAmount(time_t tStart);    //声明getAmount()函数
//添加卡
int addCardInfo(Card card)
{
	return addCard(card);

}
//声明函数
Card* queryCardInfo(const char* pName) 
{

	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;

}
//查询卡
Card* queryCardsInfo(const char* pName, int* pIndex)
{
	Card* pCard = NULL;
	pCard = queryCards(pName, pIndex);
	return pCard;
}
int initCardList();

//上机
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;     //卡信息在链表中的索引
	Billing billing;    //计费信息

	//根据卡号和密码，从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果卡信息为空，表示没有该卡信息，上机失败
	if (pCard == NULL)
	{
		printf("--**卡号或密码输入错误，请重新输入**--\n");
		return FALSE;
	}
	//如果卡状态不为0，表示该卡不能上机
	if (pCard->nStatus == 1)
	{
		return UNUSE;
	}
	if (pCard->nStatus == 2 )
	{
		return ANNULED;
	}
	//如果卡余额为0，不能上机
	if (pCard->fBalance <= 0)
	{
		return ENOUGHMONEY;
	}

	//如果可以上机，更新卡信息
	pCard->nStatus = 1;       //状态为正在使用
	pCard->nUseCount++;      //使用次数+1
	pCard->tStart = time(NULL);
	pCard->tLastUse = time(NULL);

	//更新文件中的卡信息
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}

	//添加消费记录
	strcpy(billing.aCardName, pName);      //上机卡号
	billing.tStart = time(NULL);           //上机时间
	billing.tEnd = 0;                      //下机时间
	billing.nStatus = 0;                   //消费状态
	billing.fAmount = 0;                   //消费金额
	billing.nDel = 0;                      //删除标识

	//将计费信息保存到文件中
	if (TRUE == saveBilling(&billing, BILLINGPATH))
	{
		//组装上机信息
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
	int nIndex = 0;               //卡信息在链表中的索引号
	int nPosition = 0;            //计费信息在链表中的索引号
	double dbAmount = 0.0;        //消费金额
	float fBalance = 0.0;         //余额

	//查询上机卡
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果卡信息为空，表示没有该卡信息，返回FALSE
	if (pCard == NULL)
	{
		printf("--**卡号或密码输入错误，请重新输入**--\n");
		return FALSE;
	}

	//判断卡是否在上机，只有正在上机的卡才能进行下机操作
	if (pCard->nStatus == 0)
	{
		return UNUSE;
	}

	if (pCard->nStatus == 2)
	{
		return ANNULED;
	}

	//根据卡号，查询计费信息
	pBilling = queryBilling(pName, &nPosition);

	//如果查询计费信息为空，表示下机失败
	if (pBilling == NULL)
	{
		printf("--**卡号或密码输入错误，请重新输入**--\n");
		return FALSE;
	}

	//计算消费金额
	dbAmount = getAmount(pCard->tStart);

	//如果余额小于消费金额，则不能进行下机
	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance < 0)
	{
		return ENOUGHMONEY;
	}
	
	//组装下机信息
	strcpy(pInfo->aCardName, pName);           //上机卡号
	pInfo->fAmount = (float)dbAmount;                 //消费金额
	pInfo->fBalance = fBalance;                //余额
	pInfo->tStart = pBilling->tStart;          //上机时间
	pInfo->tEnd = time(NULL);                  //下机时间

	//更新卡信息
	pCard->fTotalUse = pCard->fTotalUse + pInfo->fAmount;  //更新链表中卡信息累计使用金额
	pCard->fBalance = fBalance;   //更新链表中卡信息的余额
	pCard->nStatus = 0;           //状态
	pCard->tLastUse = time(NULL); //上次使用时间

	//更新文件中的卡信息
	if (FALSE == updateCard(pCard, CARDPATH, nIndex))
	{
		return FALSE;
	}

	//更新消费信息
	pBilling->fAmount = (float)dbAmount;    //消费信息
	pBilling->nStatus = 1;                  //状态-已结算
	pBilling->tEnd = time(NULL);            //下机时间

	//更新文件中的卡信息
	if (FALSE == updateBilling(pBilling, BILLINGPATH, nPosition))
	{
		return FALSE;
	}

	return TRUE;
}

double getAmount(time_t  tStart)
{
	int	nSec = 0;               //消费时间（秒）
	int nMin = 0;               //消费时间（分钟）
	int nCount = 0;             //上机的时间单元数，每个单元15分钟
	double dbAmount = 0;        //消费金额
	time_t tEnd = time(NULL);   //结算时间为当前时间

	//计算消费时长
	nSec = (int)(tEnd - tStart);  //秒数
	if (nSec % 60 == 0)
	{
		nMin = nSec / 60;    //整除
	}
	else
	{
		nMin = nSec / 60 + 1; //不整除
	}

	//计算消费的时间单元数
	if (nMin % UNIT == 0)
	{
		nCount = nMin / UNIT;    //整除
	}
	else
	{
		nCount = nMin / UNIT + 1; //不整除
	}

	//计算消费金额
	dbAmount = nCount * CHARGE;

	return dbAmount;
}

void releaseList()
{
	releaseCardList();            //释放卡信息链表内存
	releaseBillingList();         //释放计费信息链表内存
}
	
Card* doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查找卡信息
	pCard= checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	}
	else if(strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{   
		//状态确认,	判断能否充值
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---该卡已注销---***----\n---**--若想继续使用请重新激活--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---该卡已过期---***----\n---**--若需要退款请直接注销卡--**---\n   --*-若想继续使用请重新激活-*--\n");
			return NULL;
		}
		//更新卡信息
		pCard->fBalance = pCard->fBalance + pMoneyInfo->fMoney;
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH,nIndex) == TRUE)
			return pCard;
	}
	
}

Card* doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查找卡信息
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//状态确认,	判断能否退费
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---该卡已注销---***----\n---**--若想继续使用请重新激活--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---该卡已过期---***----\n---**--若需要退款请直接注销卡--**---\n   --*-若想继续使用请重新激活-*--\n");
			return NULL;
		}
		//更新卡信息
		if(pMoneyInfo->fMoney<= pCard->fBalance)
		{
			pCard->fBalance = pCard->fBalance - pMoneyInfo->fMoney;
		}
		else
		{
			printf("\n--**卡中余额不足退费**--\n");
			return NULL;
		}
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}
Card* doAnnul(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查询卡信息
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	} 
	else if(strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//状态确认,	判断能否注销
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---该卡已注销---***----\n---**--若想继续使用请重新激活--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---该卡已过期---***----\n---**--若需要退款请直接注销卡--**---\n   --*-若想继续使用请重新激活-*--\n");
			return NULL;
		}
		//更新卡信息:余额、最后使用时间、状态、删除标识
		pCard->fBalance = 0;
		pCard->nStatus = 2;
		pCard->nDel = 1;
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doRenew(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查询卡信息
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//状态确认,	判断能否充值
		if (pCard->nStatus != 2)
		{
			printf("\n----***---该卡未注销---***----\n");
			return NULL;
		}
		//更新卡信息:余额、最后使用时间、状态、删除标识
		pCard->nStatus = 0;
		pCard->nDel = 0;
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doChange(const char* pName,const char* pPwd,const char* cPwd) 
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查询卡信息
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
		//状态确认,	判断能否修改密码
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---该卡已注销---***----\n---**--若想继续使用请重新激活--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---该卡已过期---***----\n---**--若需要退款请直接注销卡--**---\n   --*-若想继续使用请重新激活-*--\n");
			return NULL;
		}
		strcpy(pCard->aPwd, cPwd);
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
	}
}

Card* doVip(const char* pName, const char* pPwd)
{
	Card* pCard = NULL;
	int nIndex = 0;
	//查询卡信息
	pCard = checkCard(pName, pPwd, &nIndex);
	if (pCard == NULL)
	{
		printf("\n--**卡号或密码输入错误**--\n");
		return NULL;

	}
	else if (strcmp(pCard->aName, pName) == 0 && strcmp(pCard->aPwd, pPwd) == 0)
	{
	//状态确认,	判断能否注册会员
		if (pCard->nVip == 1)
		{
			printf("\n----***---该卡已是会员---***----\n");
			return NULL;
		}
		if (pCard->nStatus == 2)
		{
			printf("\n   ----***---该卡已注销---***----\n---**--若想继续使用请重新激活--**---\n");
			return NULL;
		}
		pCard->tLastUse = time(NULL);
		if (pCard->tLastUse - pCard->tEnd >= 0)
		{
			printf("\n   ----***---该卡已过期---***----\n---**--若需要退款请直接注销卡--**---\n   --*-若想继续使用请重新激活-*--\n");
			return NULL;
		}
		pCard->nVip = 1;
		//更新卡文件信息
		if (updateCard(pCard, CARDPATH, nIndex) == TRUE)
			return pCard;
		}
}

//打印二维码函数
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
	
	printf("请按Ctrl+鼠标滚轮进行缩放，调整到合适大小\n");
	printf("充值完成后按任意键结束\n");
	getchar();
	fclose(fp);
	return 1;
}

void showcard(Card* pCard)
{
	char aLastTime[50];
	printf("卡号\t\t状态\t\t余额\t\t累计使用\t\t使用次数\t\t上次使用时间\n");
	timeToString(pCard->tLastUse, aLastTime);
	printf("%s\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", pCard->aName, pCard->nStatus,
		pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, aLastTime);
}
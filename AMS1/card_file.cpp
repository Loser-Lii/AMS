#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>    //包含文件结构体头文件
#include <string.h>   //包含字符处理头文件
#include <stdlib.h>   //包含standard library标准库头文件

#include "tool.h"     //包含工具头文件
#include "model.h"    //包含数据类型定义头文件
#include "global.h"   //包含全局定义头文件

#define CARDCHARNUM 256  //从卡信息文件中读取的字符数


int saveCard(const Card* pCard, const char* pPath)
{
	FILE* fp = NULL;     //文件结构体指针

	char startTime[TIMELENTH] = { 0 };     //开卡时间
	char endTime[TIMELENTH] = { 0 };       //截止时间
	char lastTime[TIMELENTH] = { 0 };      //最后使用时间

	//以追加的模式打开文件，如果打开失败，则以只写的模式打开文件
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		if ((fp = fopen(pPath, "w")) == NULL)
		{
			return FALSE;
		}
	}
	//将时间转换为字符串
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastUse, lastTime);
	//将数据写进文件

	fprintf(fp, "%s##%s##%d##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n", pCard->aName, pCard->aPwd,pCard->nVip,
		pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//关闭文件
	fclose(fp);
	return TRUE;
}

Card parseCard(char* pBuf)           //解析函数
{
	Card pCard;   //保存解析出来的卡信息
	const char* delims = "##";    //字符串中的分隔符
	char flag[11][20] = { 0 };     //保存分割后的字符串(*pCard)
	int index = 0;                 //指示flag数组下标
	char* buf = NULL;
	char* str = NULL;


	buf = pBuf;    //第一次调用strok时，buf为解析字符串
	while ((str = strtok(buf, delims)) != NULL)
	{
		strcpy(flag[index], str);
		buf = NULL;     //后面调用strtok函数时，第一个参数为NULL
		index++;
	}

	strcpy(pCard.aName, flag[0]);
	strcpy(pCard.aPwd, flag[1]);
	pCard.nVip = atoi(flag[2]);
	pCard.nStatus = atoi(flag[3]);
	pCard.tStart = stringToTime(flag[4]);
	pCard.tEnd = stringToTime(flag[5]);
	pCard.fTotalUse = (float)atof(flag[6]);
	pCard.tLastUse = stringToTime(flag[7]);
	pCard.nUseCount = atoi(flag[8]);
	pCard.fBalance = (float)atof(flag[9]);
	pCard.nDel = atoi(flag[10]);

	return pCard;
}

int readCard(Card* pCard, const char* pPath)
{
	FILE* fp = NULL;
	char aBuf[CARDCHARNUM] = { 0 };
	int i = 0;
	//打开文件
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);  //清空数组
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				pCard[i] = parseCard(aBuf);
				i++;
			}
		}
	}
	//关闭文件
	fclose(fp);

	return TRUE;
}


int getCardCount(const char* pPath)
{
	FILE* fp = NULL;                  //文件指针
	int nCount = 0;                   //数组的索引号
	char aBuf[CARDCHARNUM] = { 0 };   //保存从文件中读取的数据

	//以只读模式打开文件
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	//读取文件
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);  //清空数组
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				nCount++;
			}
		}
	}
	//关闭文件
	//fclose(fp);

	return nCount;
}
int updateCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;       //文件指针
	char aStart[30];       //存放转换后的时间字符串
	char aEnd[30];         //存放转换后的时间字符串
	char aLast[30];        //存放转换后的时间字符串
	int nLine = 0;         //文件中当前卡序号（行）
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;    //文件位置标记

	timeToString(pCard->tStart,aStart);
	timeToString(pCard->tEnd,aEnd);
	timeToString(pCard->tLastUse, aLast);

	//以只读方式打开文件
	fp = fopen(pPath, "r+");
	if (fp == NULL)
	{
		return FALSE;
	}

	//遍历文件，找到该条记录，进行更新
	while (!feof(fp) && nLine < nIndex)
	{
		memset(aBuf, 0, CARDCHARNUM);   //清空字符数组
		//逐行读取文件内容
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			lPosition = ftell(fp);
			nLine++;
		}

	}
	//移到文件标识位置
	fseek(fp, lPosition, 0);
	//按指定规格将信息更新到文件
	fprintf(fp, "%s##%s##%d##%d##%s##%s##%.1f##%s##%d##%.1f##%d", pCard->aName, pCard->aPwd,pCard->nVip,
		pCard->nStatus, aStart, aEnd, pCard->fTotalUse, aLast, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//关闭文件
	fclose(fp);

	return TRUE;
}

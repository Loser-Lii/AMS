#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "model.h"

//将计费信息保存到billing.ams文件中
int saveBilling(const Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;

	//以追加的模式打开文件，如果打开失败，则以只写的模式打开文件
	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			return FALSE;
		}
	}

	//将数据写入二进制文件
	fwrite(pBilling, sizeof(Billing), 1, fp);

	//关闭文件
	fclose(fp);
	return TRUE;
}

int readBilling(Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;
	int nIndex = 0;

	//以只读方式打开二进制文件，如果打开失败，则读取失败
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("----***---文件无法打开  不能读取计费信息---***----\n");
		return FALSE;
	}

	//读取文件内容
	while (!feof(fp))
	{
		if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp) != 0)
		{
			nIndex++;
		}

	}

	//关闭文件
	fclose(fp);

	return TRUE;
}

int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	//以只读方式打开二进制文件，如果打开失败，则读取失败
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("----***---文件无法打开  不能读取计费信息---***----\n");
		return FALSE;
	}

	//读取文件内容
	while (!feof(fp))
	{
		if (fread(pBilling, sizeof(Billing), 1, fp) != 0)
		{
			nCount++;
		}
	}

	//关闭文件
	fclose(fp);
	free(pBilling);
	return nCount;
}

int updateBilling(Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nCount = 0;     //读取数量
	long lPosition = 0; //文件标记位置
	Billing pbBuf;

	//以读写方式打开二进制文件
	fp = fopen(pPath, "rb+");
	if (fp == NULL)
	{
		printf("----***---文件无法打开  不能更新计费信息---***----\n");
		return FALSE;
	}

	//遍历文件，获取消费信息在文件中的位置
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0)
		{
			lPosition = ftell(fp);
		}
		nCount++;
	}

	//移动文件标记位置，到要更新的计费信息开头
	fseek(fp, lPosition, 0);

	//更新计费信息到文件
	fwrite(pBilling, sizeof(Billing), 1, fp);
	//关闭文件
	fclose(fp);

	return TRUE;
}
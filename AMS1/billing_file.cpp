#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "model.h"

//���Ʒ���Ϣ���浽billing.ams�ļ���
int saveBilling(const Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;

	//��׷�ӵ�ģʽ���ļ��������ʧ�ܣ�����ֻд��ģʽ���ļ�
	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL)
		{
			return FALSE;
		}
	}

	//������д��������ļ�
	fwrite(pBilling, sizeof(Billing), 1, fp);

	//�ر��ļ�
	fclose(fp);
	return TRUE;
}

int readBilling(Billing* pBilling, const char* pPath)
{
	FILE* fp = NULL;
	int nIndex = 0;

	//��ֻ����ʽ�򿪶������ļ��������ʧ�ܣ����ȡʧ��
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("----***---�ļ��޷���  ���ܶ�ȡ�Ʒ���Ϣ---***----\n");
		return FALSE;
	}

	//��ȡ�ļ�����
	while (!feof(fp))
	{
		if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp) != 0)
		{
			nIndex++;
		}

	}

	//�ر��ļ�
	fclose(fp);

	return TRUE;
}

int getBillingCount(const char* pPath)
{
	FILE* fp = NULL;
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	//��ֻ����ʽ�򿪶������ļ��������ʧ�ܣ����ȡʧ��
	fp = fopen(pPath, "rb");
	if (fp == NULL)
	{
		printf("----***---�ļ��޷���  ���ܶ�ȡ�Ʒ���Ϣ---***----\n");
		return FALSE;
	}

	//��ȡ�ļ�����
	while (!feof(fp))
	{
		if (fread(pBilling, sizeof(Billing), 1, fp) != 0)
		{
			nCount++;
		}
	}

	//�ر��ļ�
	fclose(fp);
	free(pBilling);
	return nCount;
}

int updateBilling(Billing* pBilling, const char* pPath, int nIndex)
{
	FILE* fp = NULL;
	int nCount = 0;     //��ȡ����
	long lPosition = 0; //�ļ����λ��
	Billing pbBuf;

	//�Զ�д��ʽ�򿪶������ļ�
	fp = fopen(pPath, "rb+");
	if (fp == NULL)
	{
		printf("----***---�ļ��޷���  ���ܸ��¼Ʒ���Ϣ---***----\n");
		return FALSE;
	}

	//�����ļ�����ȡ������Ϣ���ļ��е�λ��
	while ((!feof(fp)) && (nCount < nIndex))
	{
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0)
		{
			lPosition = ftell(fp);
		}
		nCount++;
	}

	//�ƶ��ļ����λ�ã���Ҫ���µļƷ���Ϣ��ͷ
	fseek(fp, lPosition, 0);

	//���¼Ʒ���Ϣ���ļ�
	fwrite(pBilling, sizeof(Billing), 1, fp);
	//�ر��ļ�
	fclose(fp);

	return TRUE;
}
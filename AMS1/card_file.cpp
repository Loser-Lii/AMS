#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>    //�����ļ��ṹ��ͷ�ļ�
#include <string.h>   //�����ַ�����ͷ�ļ�
#include <stdlib.h>   //����standard library��׼��ͷ�ļ�

#include "tool.h"     //��������ͷ�ļ�
#include "model.h"    //�����������Ͷ���ͷ�ļ�
#include "global.h"   //����ȫ�ֶ���ͷ�ļ�

#define CARDCHARNUM 256  //�ӿ���Ϣ�ļ��ж�ȡ���ַ���


int saveCard(const Card* pCard, const char* pPath)
{
	FILE* fp = NULL;     //�ļ��ṹ��ָ��

	char startTime[TIMELENTH] = { 0 };     //����ʱ��
	char endTime[TIMELENTH] = { 0 };       //��ֹʱ��
	char lastTime[TIMELENTH] = { 0 };      //���ʹ��ʱ��

	//��׷�ӵ�ģʽ���ļ��������ʧ�ܣ�����ֻд��ģʽ���ļ�
	if ((fp = fopen(pPath, "a")) == NULL)
	{
		if ((fp = fopen(pPath, "w")) == NULL)
		{
			return FALSE;
		}
	}
	//��ʱ��ת��Ϊ�ַ���
	timeToString(pCard->tStart, startTime);
	timeToString(pCard->tEnd, endTime);
	timeToString(pCard->tLastUse, lastTime);
	//������д���ļ�

	fprintf(fp, "%s##%s##%d##%d##%s##%s##%.1f##%s##%d##%.1f##%d\n", pCard->aName, pCard->aPwd,pCard->nVip,
		pCard->nStatus, startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//�ر��ļ�
	fclose(fp);
	return TRUE;
}

Card parseCard(char* pBuf)           //��������
{
	Card pCard;   //������������Ŀ���Ϣ
	const char* delims = "##";    //�ַ����еķָ���
	char flag[11][20] = { 0 };     //����ָ����ַ���(*pCard)
	int index = 0;                 //ָʾflag�����±�
	char* buf = NULL;
	char* str = NULL;


	buf = pBuf;    //��һ�ε���strokʱ��bufΪ�����ַ���
	while ((str = strtok(buf, delims)) != NULL)
	{
		strcpy(flag[index], str);
		buf = NULL;     //�������strtok����ʱ����һ������ΪNULL
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
	//���ļ�
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);  //�������
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				pCard[i] = parseCard(aBuf);
				i++;
			}
		}
	}
	//�ر��ļ�
	fclose(fp);

	return TRUE;
}


int getCardCount(const char* pPath)
{
	FILE* fp = NULL;                  //�ļ�ָ��
	int nCount = 0;                   //�����������
	char aBuf[CARDCHARNUM] = { 0 };   //������ļ��ж�ȡ������

	//��ֻ��ģʽ���ļ�
	fp = fopen(pPath, "r");
	if (fp == NULL)
	{
		return FALSE;
	}
	//��ȡ�ļ�
	while (!feof(fp))
	{
		memset(aBuf, 0, CARDCHARNUM);  //�������
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			if (strlen(aBuf) > 0)
			{
				nCount++;
			}
		}
	}
	//�ر��ļ�
	//fclose(fp);

	return nCount;
}
int updateCard(const Card* pCard, const char* pPath, int nIndex)
{
	FILE* fp = NULL;       //�ļ�ָ��
	char aStart[30];       //���ת�����ʱ���ַ���
	char aEnd[30];         //���ת�����ʱ���ַ���
	char aLast[30];        //���ת�����ʱ���ַ���
	int nLine = 0;         //�ļ��е�ǰ����ţ��У�
	char aBuf[CARDCHARNUM] = { 0 };
	long lPosition = 0;    //�ļ�λ�ñ��

	timeToString(pCard->tStart,aStart);
	timeToString(pCard->tEnd,aEnd);
	timeToString(pCard->tLastUse, aLast);

	//��ֻ����ʽ���ļ�
	fp = fopen(pPath, "r+");
	if (fp == NULL)
	{
		return FALSE;
	}

	//�����ļ����ҵ�������¼�����и���
	while (!feof(fp) && nLine < nIndex)
	{
		memset(aBuf, 0, CARDCHARNUM);   //����ַ�����
		//���ж�ȡ�ļ�����
		if (fgets(aBuf, CARDCHARNUM, fp) != NULL)
		{
			lPosition = ftell(fp);
			nLine++;
		}

	}
	//�Ƶ��ļ���ʶλ��
	fseek(fp, lPosition, 0);
	//��ָ�������Ϣ���µ��ļ�
	fprintf(fp, "%s##%s##%d##%d##%s##%s##%.1f##%s##%d##%.1f##%d", pCard->aName, pCard->aPwd,pCard->nVip,
		pCard->nStatus, aStart, aEnd, pCard->fTotalUse, aLast, pCard->nUseCount, pCard->fBalance, pCard->nDel);
	//�ر��ļ�
	fclose(fp);

	return TRUE;
}

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "card_service.h"
#include "global.h"
#include "model.h"
#include "tool.h"


void onedaypersons()
{
	lpCardNode cardNode = NULL;
	int count = 0;
	char searchday[12] = { 0 };
	printf("�����������ѯ�����ڷ�Χ(�밴��20xx-xx-xx�ĸ�ʽ����):");
	scanf("%s", searchday);
    count=queryDates(searchday);
	printf("\n��ѯ��Χ���������ѵ��û�������: %d ��\n", count);
}

void monthmost()
{
	int index = 0;
	int day[31];
	int month;
	int max = 0;
	char monthdates[4][31][20] = { MONTH };
	printf("��������Ҫ��ѯ���·�:");
	scanf("%d", &month);
	for(int i=0;i<31;i++)
	{
		day[i] = queryDates(monthdates[month-1][i]);
	}
	for (int i = 0; i < 31; i++)
	{
		if (day[i] > max)
		{
			max = day[i];
			index = i;
		}
	}
	printf("\n%d��������������������:%s, ��%d��\n", month,monthdates[month-1][index],max);
}

void amount()
{
	float amount;
	char beTime[20];
	char endTime[20];
	printf("������ͳ�Ƶ���ʼ����:");
	scanf("%s", beTime);
	printf("������ͳ�ƵĽ�������:");
	scanf("%s", endTime);
	amount=queryCardtime(beTime,endTime);
	printf("\n%s��%s֮����Ӫҵ��Ϊ:%.1f", beTime, endTime, amount);


}
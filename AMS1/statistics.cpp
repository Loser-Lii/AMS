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
	printf("请输入你想查询的日期范围(请按照20xx-xx-xx的格式输入):");
	scanf("%s", searchday);
    count=queryDates(searchday);
	printf("\n查询范围内来店消费的用户人数有: %d 人\n", count);
}

void monthmost()
{
	int index = 0;
	int day[31];
	int month;
	int max = 0;
	char monthdates[4][31][20] = { MONTH };
	printf("请输入你要查询的月份:");
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
	printf("\n%d月消费人数最多的日期是:%s, 有%d人\n", month,monthdates[month-1][index],max);
}

void amount()
{
	float amount;
	char beTime[20];
	char endTime[20];
	printf("请输入统计的起始日期:");
	scanf("%s", beTime);
	printf("请输入统计的结束日期:");
	scanf("%s", endTime);
	amount=queryCardtime(beTime,endTime);
	printf("\n%s到%s之间总营业额为:%.1f", beTime, endTime, amount);


}
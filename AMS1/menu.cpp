#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>      //包含时间头文件
#include <string.h>
#include <stdlib.h>  

#include "model.h"
#include "card_file.h"
#include "card_service.h" 
#include "tool.h"
#include "global.h"
#include "service.h"
#include "statistics.h"
int getSize(const char* pInfo);
#pragma warning(disable:4996)
void outputMenu()
{
    printf("\n----***---菜单---***----\n");
    printf("1.添加卡\n");
    printf("2.查询卡\n");
    printf("3.上机\n");
    printf("4.下机\n");
    printf("5.充值\n");
    printf("6.退费\n");
    printf("7.查询统计\n");
    printf("8.注销卡\n");
    printf("9.激活卡\n");
    printf("10.修改密码\n");
    printf("11.注册会员\n");
    printf("0.退出\n");
    printf("请选择菜单项编号<0-11>:");

}
void add()
{
    char aName[32] = { '\0' };
    char aPwd[20] = { '\0' };
    char c;
    Card card;
    struct tm* endTime;       //截止时间
    struct tm* startTime;     //开卡时间

    printf("\n----------添加卡----------\n");

    printf("请输入卡号<长度为1-18>:");
    scanf("%s", aName);

    //判断输入的卡号是否符合要求
    if (getSize(aName) > 18)
    {
        printf("输入的卡号长度超过最大值!\n");
        return;
    }
    //判断输入的卡号是否已存在
    if (queryCard(aName) != NULL)
    {
        printf("输入的卡号已存在!请重新输入!\n");
        return;
    }

    //将输入的卡号保存到卡结构体
    strcpy(card.aName, aName);

    printf("请输入密码<长度为1-8>:");
    scanf("%s", aPwd);

    //判断输入的密码是否符合要求
    if (getSize(aPwd) > 8)
    {
        printf("输入的密码长度超过最大值!\n");
        return;
    }
    //将输入的密码保存到卡结构体
    strcpy(card.aPwd, aPwd);
    printf("是否注册成为会员?(y or n)\n");
    getchar();
    printf("你的选择是:");
    scanf("%c", &c);
    if (c == 'y')
    {
        card.nVip = 1;
        printf("已成功注册会员\n");
    }
    else if (c == 'n')
    {
        card.nVip = 0;
        printf("非会员\n");
    }
    printf("请输入开卡金额<RMB>:");
    scanf("%f", &card.fBalance);

    card.fTotalUse = 0;               //添加卡时，累计金额为0
    card.nUseCount = 0;                //使用次数
    card.nStatus = 0;                  //卡状态
    card.nDel = 0;                     //删除标识
    card.tStart = card.tEnd = card.tLastUse = time(NULL);  //开卡时间，截止时间，最后时间都默认

    //根据开卡时间，计算截止时间，每张卡的有效期为一年
    startTime = localtime(&card.tStart);
    endTime = localtime(&card.tEnd);
    endTime->tm_year = startTime->tm_year + 1;
    card.tEnd = mktime(endTime);

    if (FALSE == addCardInfo(card))
    {
        printf("---**-添加卡信息失败-**---\n");
    }
    else
    {
        printf("---**-添加卡信息成功-**---\n");
    }
    //输出卡信息
    printf("\n------添加的卡信息如下------\n");
    printf("卡号\t密码\tVIP状态\t卡状态\t余额\n");
    printf("%s\t%s\t%d\t%d\t%.1f\n", card.aName, card.aPwd, card.nVip,card.nStatus, card.fBalance);

}
int getSize(const char* pInfo)
{
    int nSize = 0;
    //计算字符串的字符个数
    while (*(pInfo + nSize) != '\0')
    {
        nSize++;
    }
    return nSize;
}
void query()
{
    char aName[18] = { 0 };    //保存输入的卡号
    Card* pCard = NULL;        //保存查询到的卡信息
    char aLastTime[TIMELENTH] = { 0 };
    int nIndex = 0;
    int choose = 0;

    printf("\n----------查询卡----------\n");
    printf("请输入查询方式<1 or 2>:\n1.精确查询\n2.模糊查询\n3.查询全部\n选择：");
    scanf("%d", &choose);
    if (choose == 3)
    {
        pCard = queryAll(&nIndex);
    }
    else
    {
        //提示并接收输入的卡信息
        printf("请输入查询的卡号<长度为1-18>:");
        scanf("%s", aName);

        //查询卡
        if (choose == 1)    //精确查询
        {
            pCard = queryCardInfo(aName);
        }
        else if (choose == 2)          //模糊查询
        {
            pCard = queryCardsInfo(aName, &nIndex);
        }

    }

    //如果pCard为NULL,表示没有该卡信息
    if (pCard == NULL )
    {
        printf("\n--**没有该卡的信息**--\n");
    }
    else
    {
        printf("\n查询到的卡信息如下:\n");
        //输出表格的表头
        printf("卡号\tVIP状态\t卡状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
        if (choose == 1)           //精确查询输出
        {
            timeToString(pCard->tLastUse, aLastTime);//将时间转换为字符串
            if (pCard->nVip == 1)
            {
                printf("\033[33m%s\t%d\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n\033[0m", pCard->aName, pCard->nVip, pCard->nStatus, pCard->fBalance,
                    pCard->fTotalUse, pCard->nUseCount, aLastTime);
            }
            else 
            {
                printf("%s\t%d\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", pCard->aName, pCard->nVip, pCard->nStatus, pCard->fBalance,
                    pCard->fTotalUse, pCard->nUseCount, aLastTime);
            }
        }
        else                       //模糊查询输出
        {
            for (int i = 0; i < nIndex; i++)
            {
                timeToString(pCard[i].tLastUse, aLastTime);//将时间转换为字符串
                if (pCard[i].nVip == 1)
                {
                    printf("\033[33m%s\t%d\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n\033[0m", pCard[i].aName, pCard[i].nVip, pCard[i].nStatus, pCard[i].fBalance,
                        pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
                }
                else 
                {
                    printf("%s\t%d\t%d\t%.1f\t%.1f\t\t%d\t\t%s\n", pCard[i].aName, pCard[i].nVip, pCard[i].nStatus, pCard[i].fBalance,
                        pCard[i].fTotalUse, pCard[i].nUseCount, aLastTime);
                }
            }
            //释放动态分配的内存
            free(pCard);
        }
        pCard = NULL;
    }

}

void logon()
{
    char aName[20] = { 0 };
    char aPwd[8] = { 0 };
    char aLastTime[TIMELENTH] = { 0 };
    LogonInfo* pInfo = NULL;
    int nResult = -1;

    //接收用户输入的卡号和密码 
    printf("\n----------上机-----------\n");

    printf("请输入卡号<长度为1-18>:");
    scanf("%s", aName);
    if (getSize(aName) >= 18)
    {
        printf("输入的卡号长度超过最大值!\n");
        return;
    }

    printf("请输入密码<长度为1-8>:");
    scanf("%s", aPwd);
    if (getSize(aPwd) >= 8)
    {
        printf("输入的密码长度超过最大值!\n");
        return;
    }

    pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

    //进行上机,根据上机结果，提示不同信息
    nResult = doLogon(aName, aPwd, pInfo);
    printf("\n上机信息如下:\n");
    switch (nResult)
    {
    case 0:
        printf("--**上机失败**--\n");
        break;
    case 1:
        printf("卡号\t余额\t上机时间\n");
        timeToString(pInfo->tLogon, aLastTime);
        //显示上机卡信息
        printf("%s\t%0.1f\t%s\n", pInfo->aCardName, pInfo->fBalance, aLastTime);
        printf("\n--**上机成功**--\n");
        break;
    case 2:
        printf("\n--**该卡正在使用**--\n");
        break;
    case 3:
        printf("\n--**余额不足**--\n");
        break;
    case 4:
        printf("\n--**该卡已注销**--\n");
    default:
        break;
    }

    //释放上机信息
    free(pInfo);
}
void settle()
{
    char aName[18] = { 0 };
    char aPwd[8] = { 0 };
    int nResult = -1;            //下机结果
    SettleInfo* pInfo = NULL;    //下机信息
    char aStartTime[TIMELENTH] = { 0 };   //上机时间
    char aEndTime[TIMELENTH] = { 0 };     //下机时间

    //为下机信息动态分配内存
    pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

    printf("\n----------下机-----------\n");

    printf("请输入下机卡号<长度为1-18>:");
    scanf("%s", aName);

    printf("请输入下机密码<长度为1-8>:");
    scanf("%s", aPwd);

    //进行下机
    nResult = doSettle(aName, aPwd, pInfo);

    //根据下机结果，提示不同信息
    printf("\n下机信息如下:\n");
    switch (nResult)
    {
    case 0:   //下机失败
    {
        printf("\n--**下机失败**--\n");
        break;
    }
    case 1:   //下机成功
    {
        printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
        //上机时间默认为卡的最后使用时间，后面添加计费信息后，使用计费信息时间
        timeToString(pInfo->tStart, aStartTime);
        timeToString(pInfo->tEnd, aEndTime);
        printf("%s\t%0.1f\t%0.1f\t%s\t%s\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, aStartTime, aEndTime);
        printf("\n--**下机成功**--\n");
        break;
    }
    case 2:   //未在使用或着已注销
        printf("\n--**该卡未在使用**--\n");
        break; 
    case 3:
        printf("\n--**卡余额不足**--\n");
        break;
    case 4:
        printf("\n--**该卡已注销**--\n");
        break;
    default:
        break;
    }
    //释放动态分配的内存
    free(pInfo);
}

void addMoney()
{
    Card* pCard = NULL;
    MoneyInfo* money=NULL;
    money = (MoneyInfo*)malloc(sizeof(MoneyInfo*));
    char aCardName[18] = { 0};
    char aPwd[8] = { 0 };
    //提示用户输入卡号、密码、充值金额

    printf("\n----------充值-----------\n");

    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);
    printf("请输入充值金额<RMB>:");

    while (scanf("%f", &money->fMoney) != 1) {
        printf("您输入的充值金额有误<请输入整数或小数>\n");
        printf("请重新输入:\n");
        while (getchar() != '\n');
    }
    //充值
    pCard=doAddMoney(aCardName, aPwd,money);
    //显示充值信息
    if (pCard == NULL)
    {
        printf("\n--**充值失败**--\n");
    }
    else
    {
        printf("\n--**充值成功**--\n");
        printf("卡号\t充值金额\t余额\n");
        printf("%s\t%.1f\t\t%.1f\n", pCard->aName, money->fMoney, pCard->fBalance);

        printQR();   //打印收款二维码
    }

}

void refundMoney()
{
    Card* pCard = NULL;
    MoneyInfo* money = NULL;
    money = (MoneyInfo*)malloc(sizeof(MoneyInfo*));
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //提示用户输入卡号、密码、退费金额
    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);
    printf("请输入退费金额<RMB>:");

    while (scanf("%f", &money->fMoney) != 1) {
        printf("\n您输入的退费金额有误<请输入整数或小数>\n");
        printf("请重新输入:\n");
        while (getchar() != '\n');
    }
    //充值
    pCard = doRefundMoney(aCardName, aPwd, money);
    //显示退费信息
    if (pCard == NULL)
    {
        printf("\n--**退费失败**--\n");
    }
    else
    {
        printf("\n--**退费成功**--\n");
        printf("卡号\t充值金额\t余额\n");
        printf("%s\t%.1f\t\t%.1f\n", pCard->aName, money->fMoney, pCard->fBalance);
    }

}

void annul()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //提示输入卡号 密码
    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);

    //注销
    pCard = doAnnul(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**注销失败**--\n");
    else
        printf("\n--**注销成功**--\n");
}

void renew()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //提示输入卡号 密码
    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);

    //注销
    pCard = doRenew(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**激活失败**--\n");
    else
        printf("\n--**激活成功**--\n");
}

void changepwd()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    char cPwd[8] = { 0 };
    //提示输入卡号 密码
    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);
    printf("请输入修改密码<长度为1-8>:");
    scanf("%s", &cPwd);
    //注销
    pCard = doChange(aCardName, aPwd,cPwd);
    if (pCard == NULL)
        printf("\n--**修改失败**--\n");
    else
        printf("\n--**修改成功**--\n");
}

void statistics()
{
    int choose = 0;
    printf("\n----------统计查询-----------\n");
    printf("1.某年或某月或某日来店消费的用户人数\n");
    printf("2.某月消费人数最多的一天\n");
    printf("3.统计某段时间内的总营业额\n");
    printf("4.开发中......(敬请期待)\n");
    printf("\n请选择你要查询的统计项目:");
    scanf("%d", &choose);
    switch(choose)
    {
    case 1:
        onedaypersons();
        break;
    case 2:
        monthmost();
        break;
    case 3:
        amount();
        break;
    default:
        printf("\n该功能尚未推出\n");
    }

}

void vip()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //提示输入卡号 密码
    printf("请输入卡号<长度为1-18>:");
    scanf("%s", &aCardName);           //接收卡号和密码
    printf("请输入密码<长度为1-8>:");
    scanf("%s", &aPwd);

    //注销
    pCard = doVip(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**注册失败**--\n");
    else
        printf("\n--**注册成功**--\n");
}

void exitApp()
{
    releaseList();
    printf("\n----***---已退出计费管理系统---***----\n");
    getchar();
}


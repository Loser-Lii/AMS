#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>      //����ʱ��ͷ�ļ�
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
    printf("\n----***---�˵�---***----\n");
    printf("1.��ӿ�\n");
    printf("2.��ѯ��\n");
    printf("3.�ϻ�\n");
    printf("4.�»�\n");
    printf("5.��ֵ\n");
    printf("6.�˷�\n");
    printf("7.��ѯͳ��\n");
    printf("8.ע����\n");
    printf("9.���\n");
    printf("10.�޸�����\n");
    printf("11.ע���Ա\n");
    printf("0.�˳�\n");
    printf("��ѡ��˵�����<0-11>:");

}
void add()
{
    char aName[32] = { '\0' };
    char aPwd[20] = { '\0' };
    char c;
    Card card;
    struct tm* endTime;       //��ֹʱ��
    struct tm* startTime;     //����ʱ��

    printf("\n----------��ӿ�----------\n");

    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", aName);

    //�ж�����Ŀ����Ƿ����Ҫ��
    if (getSize(aName) > 18)
    {
        printf("����Ŀ��ų��ȳ������ֵ!\n");
        return;
    }
    //�ж�����Ŀ����Ƿ��Ѵ���
    if (queryCard(aName) != NULL)
    {
        printf("����Ŀ����Ѵ���!����������!\n");
        return;
    }

    //������Ŀ��ű��浽���ṹ��
    strcpy(card.aName, aName);

    printf("����������<����Ϊ1-8>:");
    scanf("%s", aPwd);

    //�ж�����������Ƿ����Ҫ��
    if (getSize(aPwd) > 8)
    {
        printf("��������볤�ȳ������ֵ!\n");
        return;
    }
    //����������뱣�浽���ṹ��
    strcpy(card.aPwd, aPwd);
    printf("�Ƿ�ע���Ϊ��Ա?(y or n)\n");
    getchar();
    printf("���ѡ����:");
    scanf("%c", &c);
    if (c == 'y')
    {
        card.nVip = 1;
        printf("�ѳɹ�ע���Ա\n");
    }
    else if (c == 'n')
    {
        card.nVip = 0;
        printf("�ǻ�Ա\n");
    }
    printf("�����뿪�����<RMB>:");
    scanf("%f", &card.fBalance);

    card.fTotalUse = 0;               //��ӿ�ʱ���ۼƽ��Ϊ0
    card.nUseCount = 0;                //ʹ�ô���
    card.nStatus = 0;                  //��״̬
    card.nDel = 0;                     //ɾ����ʶ
    card.tStart = card.tEnd = card.tLastUse = time(NULL);  //����ʱ�䣬��ֹʱ�䣬���ʱ�䶼Ĭ��

    //���ݿ���ʱ�䣬�����ֹʱ�䣬ÿ�ſ�����Ч��Ϊһ��
    startTime = localtime(&card.tStart);
    endTime = localtime(&card.tEnd);
    endTime->tm_year = startTime->tm_year + 1;
    card.tEnd = mktime(endTime);

    if (FALSE == addCardInfo(card))
    {
        printf("---**-��ӿ���Ϣʧ��-**---\n");
    }
    else
    {
        printf("---**-��ӿ���Ϣ�ɹ�-**---\n");
    }
    //�������Ϣ
    printf("\n------��ӵĿ���Ϣ����------\n");
    printf("����\t����\tVIP״̬\t��״̬\t���\n");
    printf("%s\t%s\t%d\t%d\t%.1f\n", card.aName, card.aPwd, card.nVip,card.nStatus, card.fBalance);

}
int getSize(const char* pInfo)
{
    int nSize = 0;
    //�����ַ������ַ�����
    while (*(pInfo + nSize) != '\0')
    {
        nSize++;
    }
    return nSize;
}
void query()
{
    char aName[18] = { 0 };    //��������Ŀ���
    Card* pCard = NULL;        //�����ѯ���Ŀ���Ϣ
    char aLastTime[TIMELENTH] = { 0 };
    int nIndex = 0;
    int choose = 0;

    printf("\n----------��ѯ��----------\n");
    printf("�������ѯ��ʽ<1 or 2>:\n1.��ȷ��ѯ\n2.ģ����ѯ\n3.��ѯȫ��\nѡ��");
    scanf("%d", &choose);
    if (choose == 3)
    {
        pCard = queryAll(&nIndex);
    }
    else
    {
        //��ʾ����������Ŀ���Ϣ
        printf("�������ѯ�Ŀ���<����Ϊ1-18>:");
        scanf("%s", aName);

        //��ѯ��
        if (choose == 1)    //��ȷ��ѯ
        {
            pCard = queryCardInfo(aName);
        }
        else if (choose == 2)          //ģ����ѯ
        {
            pCard = queryCardsInfo(aName, &nIndex);
        }

    }

    //���pCardΪNULL,��ʾû�иÿ���Ϣ
    if (pCard == NULL )
    {
        printf("\n--**û�иÿ�����Ϣ**--\n");
    }
    else
    {
        printf("\n��ѯ���Ŀ���Ϣ����:\n");
        //������ı�ͷ
        printf("����\tVIP״̬\t��״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
        if (choose == 1)           //��ȷ��ѯ���
        {
            timeToString(pCard->tLastUse, aLastTime);//��ʱ��ת��Ϊ�ַ���
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
        else                       //ģ����ѯ���
        {
            for (int i = 0; i < nIndex; i++)
            {
                timeToString(pCard[i].tLastUse, aLastTime);//��ʱ��ת��Ϊ�ַ���
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
            //�ͷŶ�̬������ڴ�
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

    //�����û�����Ŀ��ź����� 
    printf("\n----------�ϻ�-----------\n");

    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", aName);
    if (getSize(aName) >= 18)
    {
        printf("����Ŀ��ų��ȳ������ֵ!\n");
        return;
    }

    printf("����������<����Ϊ1-8>:");
    scanf("%s", aPwd);
    if (getSize(aPwd) >= 8)
    {
        printf("��������볤�ȳ������ֵ!\n");
        return;
    }

    pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));

    //�����ϻ�,�����ϻ��������ʾ��ͬ��Ϣ
    nResult = doLogon(aName, aPwd, pInfo);
    printf("\n�ϻ���Ϣ����:\n");
    switch (nResult)
    {
    case 0:
        printf("--**�ϻ�ʧ��**--\n");
        break;
    case 1:
        printf("����\t���\t�ϻ�ʱ��\n");
        timeToString(pInfo->tLogon, aLastTime);
        //��ʾ�ϻ�����Ϣ
        printf("%s\t%0.1f\t%s\n", pInfo->aCardName, pInfo->fBalance, aLastTime);
        printf("\n--**�ϻ��ɹ�**--\n");
        break;
    case 2:
        printf("\n--**�ÿ�����ʹ��**--\n");
        break;
    case 3:
        printf("\n--**����**--\n");
        break;
    case 4:
        printf("\n--**�ÿ���ע��**--\n");
    default:
        break;
    }

    //�ͷ��ϻ���Ϣ
    free(pInfo);
}
void settle()
{
    char aName[18] = { 0 };
    char aPwd[8] = { 0 };
    int nResult = -1;            //�»����
    SettleInfo* pInfo = NULL;    //�»���Ϣ
    char aStartTime[TIMELENTH] = { 0 };   //�ϻ�ʱ��
    char aEndTime[TIMELENTH] = { 0 };     //�»�ʱ��

    //Ϊ�»���Ϣ��̬�����ڴ�
    pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

    printf("\n----------�»�-----------\n");

    printf("�������»�����<����Ϊ1-18>:");
    scanf("%s", aName);

    printf("�������»�����<����Ϊ1-8>:");
    scanf("%s", aPwd);

    //�����»�
    nResult = doSettle(aName, aPwd, pInfo);

    //�����»��������ʾ��ͬ��Ϣ
    printf("\n�»���Ϣ����:\n");
    switch (nResult)
    {
    case 0:   //�»�ʧ��
    {
        printf("\n--**�»�ʧ��**--\n");
        break;
    }
    case 1:   //�»��ɹ�
    {
        printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
        //�ϻ�ʱ��Ĭ��Ϊ�������ʹ��ʱ�䣬������ӼƷ���Ϣ��ʹ�üƷ���Ϣʱ��
        timeToString(pInfo->tStart, aStartTime);
        timeToString(pInfo->tEnd, aEndTime);
        printf("%s\t%0.1f\t%0.1f\t%s\t%s\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, aStartTime, aEndTime);
        printf("\n--**�»��ɹ�**--\n");
        break;
    }
    case 2:   //δ��ʹ�û�����ע��
        printf("\n--**�ÿ�δ��ʹ��**--\n");
        break; 
    case 3:
        printf("\n--**������**--\n");
        break;
    case 4:
        printf("\n--**�ÿ���ע��**--\n");
        break;
    default:
        break;
    }
    //�ͷŶ�̬������ڴ�
    free(pInfo);
}

void addMoney()
{
    Card* pCard = NULL;
    MoneyInfo* money=NULL;
    money = (MoneyInfo*)malloc(sizeof(MoneyInfo*));
    char aCardName[18] = { 0};
    char aPwd[8] = { 0 };
    //��ʾ�û����뿨�š����롢��ֵ���

    printf("\n----------��ֵ-----------\n");

    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);
    printf("�������ֵ���<RMB>:");

    while (scanf("%f", &money->fMoney) != 1) {
        printf("������ĳ�ֵ�������<������������С��>\n");
        printf("����������:\n");
        while (getchar() != '\n');
    }
    //��ֵ
    pCard=doAddMoney(aCardName, aPwd,money);
    //��ʾ��ֵ��Ϣ
    if (pCard == NULL)
    {
        printf("\n--**��ֵʧ��**--\n");
    }
    else
    {
        printf("\n--**��ֵ�ɹ�**--\n");
        printf("����\t��ֵ���\t���\n");
        printf("%s\t%.1f\t\t%.1f\n", pCard->aName, money->fMoney, pCard->fBalance);

        printQR();   //��ӡ�տ��ά��
    }

}

void refundMoney()
{
    Card* pCard = NULL;
    MoneyInfo* money = NULL;
    money = (MoneyInfo*)malloc(sizeof(MoneyInfo*));
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //��ʾ�û����뿨�š����롢�˷ѽ��
    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);
    printf("�������˷ѽ��<RMB>:");

    while (scanf("%f", &money->fMoney) != 1) {
        printf("\n��������˷ѽ������<������������С��>\n");
        printf("����������:\n");
        while (getchar() != '\n');
    }
    //��ֵ
    pCard = doRefundMoney(aCardName, aPwd, money);
    //��ʾ�˷���Ϣ
    if (pCard == NULL)
    {
        printf("\n--**�˷�ʧ��**--\n");
    }
    else
    {
        printf("\n--**�˷ѳɹ�**--\n");
        printf("����\t��ֵ���\t���\n");
        printf("%s\t%.1f\t\t%.1f\n", pCard->aName, money->fMoney, pCard->fBalance);
    }

}

void annul()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //��ʾ���뿨�� ����
    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);

    //ע��
    pCard = doAnnul(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**ע��ʧ��**--\n");
    else
        printf("\n--**ע���ɹ�**--\n");
}

void renew()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //��ʾ���뿨�� ����
    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);

    //ע��
    pCard = doRenew(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**����ʧ��**--\n");
    else
        printf("\n--**����ɹ�**--\n");
}

void changepwd()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    char cPwd[8] = { 0 };
    //��ʾ���뿨�� ����
    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);
    printf("�������޸�����<����Ϊ1-8>:");
    scanf("%s", &cPwd);
    //ע��
    pCard = doChange(aCardName, aPwd,cPwd);
    if (pCard == NULL)
        printf("\n--**�޸�ʧ��**--\n");
    else
        printf("\n--**�޸ĳɹ�**--\n");
}

void statistics()
{
    int choose = 0;
    printf("\n----------ͳ�Ʋ�ѯ-----------\n");
    printf("1.ĳ���ĳ�»�ĳ���������ѵ��û�����\n");
    printf("2.ĳ��������������һ��\n");
    printf("3.ͳ��ĳ��ʱ���ڵ���Ӫҵ��\n");
    printf("4.������......(�����ڴ�)\n");
    printf("\n��ѡ����Ҫ��ѯ��ͳ����Ŀ:");
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
        printf("\n�ù�����δ�Ƴ�\n");
    }

}

void vip()
{
    Card* pCard = NULL;
    int n = 0;
    char aCardName[18] = { 0 };
    char aPwd[8] = { 0 };
    //��ʾ���뿨�� ����
    printf("�����뿨��<����Ϊ1-18>:");
    scanf("%s", &aCardName);           //���տ��ź�����
    printf("����������<����Ϊ1-8>:");
    scanf("%s", &aPwd);

    //ע��
    pCard = doVip(aCardName, aPwd);
    if (pCard == NULL)
        printf("\n--**ע��ʧ��**--\n");
    else
        printf("\n--**ע��ɹ�**--\n");
}

void exitApp()
{
    releaseList();
    printf("\n----***---���˳��Ʒѹ���ϵͳ---***----\n");
    getchar();
}


#ifndef MODEL_H
#define MODEL_H
#include <time.h>

typedef struct Card
{
	char aName[18];   //����
	char aPwd[8];     //������	
	int nStatus;      //��״̬��0-δ�ϻ���1-�����ϻ���2-��ע����3-ʧЧ��
	float fAmount;    //�ۼƽ��
	int nUseCount;    //ʹ�ô���
	float fBalance;   //���
	float exBalance;  //��Ա��ֵ�������
	float fTotalUse;  //�ۼƽ��
	int nDel;         //ɾ����ʶ��0-δɾ����1-ɾ����
	int nVip;         //VIP��ʶ

	time_t tStart;    //����ʱ��
	time_t tEnd;      //��ֹʱ��
	time_t tLastUse;  //���ʹ��ʱ��

}Card;

typedef struct CardNode {
	Card data;
	struct CardNode* next;
}CardNode, * lpCardNode;

typedef struct Billing
{
	char aCardName[18];    //����
	time_t tStart;         //�ϻ�ʱ��
	time_t tEnd;           //�»�ʱ��
	float fAmount;		   //���ѽ��
	int nStatus;           //����״̬��0-δ���㣬1-�ѽ���
	int nDel;              //ɾ����ʶ��0-δɾ����1-��ɾ��
}Billing;

typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode, * lpBillingNode;

typedef struct LogonInfo
{
	char aCardName[18];     //�ϻ�����
	time_t tLogon;			//�ϻ�ʱ��
	float fBalance;		    //�ϻ�ʱ�������
} LogonInfo;

typedef struct SettleInfo
{
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	float fAmount;
	float fBalance;
}SettleInfo;

typedef struct Money
{
	char aCardName[18];
	time_t tTime;     //��ֵ�˷ѵ�ʱ��
	int nStatus;      //״̬��0-��ʾ��ֵ��1-��ʾ�˷�
	float fMoney;     //��ֵ�˷ѽ��
	int nDel;         //ɾ����ʶ��0-δɾ����1-ɾ��
}Money;

typedef struct MoneyInfo
{
	char aCardName[18];
	float fMoney;        //��ֵ�˷ѽ��
	float fBalance;      //���
}MoneyInfo;
#endif

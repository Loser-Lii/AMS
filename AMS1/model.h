#ifndef MODEL_H
#define MODEL_H
#include <time.h>

typedef struct Card
{
	char aName[18];   //卡号
	char aPwd[8];     //卡密码	
	int nStatus;      //卡状态（0-未上机；1-正在上机；2-已注销；3-失效）
	float fAmount;    //累计金额
	int nUseCount;    //使用次数
	float fBalance;   //余额
	float exBalance;  //会员充值赠送余额
	float fTotalUse;  //累计金额
	int nDel;         //删除标识（0-未删除，1-删除）
	int nVip;         //VIP标识

	time_t tStart;    //开卡时间
	time_t tEnd;      //截止时间
	time_t tLastUse;  //最后使用时间

}Card;

typedef struct CardNode {
	Card data;
	struct CardNode* next;
}CardNode, * lpCardNode;

typedef struct Billing
{
	char aCardName[18];    //卡号
	time_t tStart;         //上机时间
	time_t tEnd;           //下机时间
	float fAmount;		   //消费金额
	int nStatus;           //消费状态，0-未结算，1-已结算
	int nDel;              //删除标识，0-未删除，1-已删除
}Billing;

typedef struct BillingNode
{
	Billing data;
	struct BillingNode* next;
}BillingNode, * lpBillingNode;

typedef struct LogonInfo
{
	char aCardName[18];     //上机卡号
	time_t tLogon;			//上机时间
	float fBalance;		    //上机时卡的余额
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
	time_t tTime;     //充值退费的时间
	int nStatus;      //状态，0-表示充值；1-表示退费
	float fMoney;     //充值退费金额
	int nDel;         //删除标识，0-未删除，1-删除
}Money;

typedef struct MoneyInfo
{
	char aCardName[18];
	float fMoney;        //充值退费金额
	float fBalance;      //余额
}MoneyInfo;
#endif

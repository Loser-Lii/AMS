#ifndef SERVICE_H
#define SERVICE_H
int addCardInfo(Card card);
Card* queryCardInfo(const char* pName);
Card* queryCardsInfo(const char* pName, int* pIndex);
int doLogon(const char* pName, const char* pPwd, LogonInfo* pLongInfo);
void releaseList();
void showcard(Card* pCard);
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
Card* doAddMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo);
Card* doRefundMoney(char* pName, char* pPwd, MoneyInfo* pMoneyInfo);
Card* doAnnul(const char* aCardName, const char* aPwd);
Card* doRenew(const char* pName, const char* pPwd);
Card* doChange(const char* pName, const char* pPwd, const char* cPwd);
Card* doVip(const char* pName, const char* pPwd);
int printQR();
#endif
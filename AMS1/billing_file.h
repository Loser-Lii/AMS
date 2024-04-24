#ifndef BILLING_FILE_H
#define BILLING_FILE_H

int saveBilling(const Billing* pBilling, const char* pPath);
int readBilling(Billing* pBilling, const char* pPath);
int getBillingCount(const char* pPath);
int updateBilling(Billing* pBilling, const char* pPath, int nIndex);

#endif
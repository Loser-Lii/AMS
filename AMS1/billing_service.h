#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H

int addBilling(const char* pName, Billing* billing);
Billing* queryBilling(const char* pName, int* pIndex);
void releaseBillingList();

#endif
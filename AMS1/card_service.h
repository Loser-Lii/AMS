#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
#include "model.h"
int addCard(Card card);  //ÉùÃ÷º¯Êý
int getCard();
Card* queryCard(const char* pName);
Card* queryCards(const char* pName, int* pIndex);
Card* queryAll(int* pIndex);
int initCardList();
void releaseCardList();
Card* checkCard(const char* pName, const char* pPwd, int* nIndex);
int queryDates(const char* pName);
float queryCardtime(char* begin, char* end);
#endif
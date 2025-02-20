#ifndef   MATCH_H
#define MATCH_H

#include <iostream>
#include <string>
#include <queue>
#include "OrderManager.h"
#include "OrderPassive.h"
#include "MarketOrder.h"

using namespace std;

class Match
{
private:
    string name;
    OrderManager orderManager;
    bool shouldPrintBook = false;

public:
    void enableBookPrinting();
    void disableBookPrinting();
    bool isBookPrintingEnabled();
    OrderManager& getOrderManager();
    void printBook( );
    void tradeActiveOrder(MarketOrder* market);
    void tradePassiveOrder(OrderPassive* orderPassive);
    Match(string name);
   virtual ~Match();
};

#endif
#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "OrderPassive.h"
#include "MarketOrder.h"

#include <algorithm>
#include <string>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

struct BuyComparator
{
    bool operator()(OrderPassive* a, OrderPassive* b) {
        if (a->getPrice() == b->getPrice()) {
            return a->getId() > b->getId();
        }
        return a->getPrice() < b->getPrice();
    }
};
struct SellComparator
{
    bool operator()(OrderPassive* a, OrderPassive* b) {
        if (a->getPrice() == b->getPrice()) {
            return a->getId() > b->getId();
        }
        return a->getPrice() > b->getPrice(); 
    }
};

class OrderManager {

protected:
 
public:
    static priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> buyLimitOrder;
    static priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> sellLimitOrder;
    static  vector<OrderPassive*> pegBuyOrders;  
    static vector<OrderPassive*> pegSellOrders; 

    static vector<OrderPassive*>& getPegBuyOrders();
    static vector<OrderPassive*>& getPegSellOrders();
    static priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator>& getBuyLimitOrder();
    static priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator>& getSellLimitOrder();
    void addlimitOrder(OrderPassive* order);
    void addpegOrder(OrderPassive* order);
    void removeOrderFromBook(OrderPassive* order);
    void cancelOrder(string id);
    void executeOrder(string id);
    void changeOrder(string id, double price, double quantity);
    void updatePegBid();
    void updatePegAsk();
    double getBestBid();
    double getBestAsk();
    OrderManager();
    virtual ~OrderManager();
};

#endif
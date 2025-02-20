#include "OrderManager.h"
#include "OrderPassive.h"
#include "Match.h"

priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> OrderManager::buyLimitOrder;
priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> OrderManager::sellLimitOrder;
vector<OrderPassive*> OrderManager::pegBuyOrders;
vector<OrderPassive*> OrderManager::pegSellOrders;

vector<OrderPassive *>& OrderManager::getPegBuyOrders() { return pegBuyOrders; }

vector<OrderPassive *>& OrderManager::getPegSellOrders() { return pegSellOrders; }

priority_queue<OrderPassive *, vector<OrderPassive *>, BuyComparator>& OrderManager::getBuyLimitOrder() { return buyLimitOrder; }

priority_queue<OrderPassive *, vector<OrderPassive *>, SellComparator>& OrderManager::getSellLimitOrder() { return sellLimitOrder; }

void OrderManager::addlimitOrder(OrderPassive *order) {
 if(order->getQuantity() > 0 ){
    if (order->getSide() == "buy") {
        buyLimitOrder.push(order);
        updatePegBid();
    } else {
         sellLimitOrder.push(order);
         updatePegAsk();
        }
    }

 }

void OrderManager::addpegOrder(OrderPassive *order) { 
 if(order->getSubtype() == "bid") {
    if(buyLimitOrder.empty()) { throw new logic_error("There is no limit in the buy book to precify a peg"); }
        pegBuyOrders.push_back(order); 
        order->setPrice(getBestBid());
    } else {
    if(sellLimitOrder.empty()) { throw new logic_error("There is no limit in the sell book to precify a peg"); }
        pegSellOrders.push_back(order);
        order->setPrice(getBestAsk());
    }
    
}

double OrderManager::getBestBid()  { return buyLimitOrder.empty() ? 0.0 : buyLimitOrder.top()->getPrice(); }

double OrderManager::getBestAsk() { return sellLimitOrder.empty() ? 0.0 : sellLimitOrder.top()->getPrice(); }

void OrderManager::removeOrderFromBook(OrderPassive* order) {
    if (order->getType() == "limit") {
        if (order->getSide() == "buy") {
            priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> temp;
            while (!buyLimitOrder.empty()) {
                if (buyLimitOrder.top() != order)
                    temp.emplace(buyLimitOrder.top());
                buyLimitOrder.pop();
            }
            buyLimitOrder = move(temp);
        } else {
            priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> temp;
            while (!sellLimitOrder.empty()) {
                if (sellLimitOrder.top() != order)
                    temp.emplace(sellLimitOrder.top());
                sellLimitOrder.pop();
            }
            sellLimitOrder = move(temp);
        }
    } else if (order->getType() == "peg") {
        if (order->getSubtype() == "bid") {
            pegBuyOrders.erase(remove(pegBuyOrders.begin(), pegBuyOrders.end(), order), pegBuyOrders.end());
        } else {
            pegSellOrders.erase(remove(pegSellOrders.begin(), pegSellOrders.end(), order), pegSellOrders.end());
        }
    }
}

void OrderManager::cancelOrder(string id) {
    OrderPassive* order = OrderPassive::cancelOrder(id);
    if (!order) return;
        removeOrderFromBook(order);
        if(order->getSide() == "buy"){
            updatePegBid();
         } else {
             updatePegAsk();
         }

    delete order;
}

void OrderManager::executeOrder(string id) { 
    OrderPassive* order = OrderPassive::executeOrder(id);
    if (!order) return;
        removeOrderFromBook(order);
        if(order->getSide() == "buy"){
             updatePegBid();
        } else {
             updatePegAsk();
        }

    delete order;
}

void OrderManager::changeOrder(string id, double price, double quantity) {
    OrderPassive* order = OrderPassive::findOrder(id);
    if (!order) return;
        removeOrderFromBook(order);
        order->setPrice(price);
         order->setQuantity(quantity);
         if (order->getType() == "limit") {
            addlimitOrder(order);
        }else {
            addpegOrder(order);
        }
}

void OrderManager::updatePegBid() {
    if(!(buyLimitOrder.empty()) && !pegBuyOrders.empty()) {
        if(pegBuyOrders[1]->getPrice() != getBestBid()){
            double bestBid = getBestBid();
            for (auto& pegOrder : pegBuyOrders) {
                pegOrder->setPrice(bestBid);
            }
        }
    }
}

void OrderManager::updatePegAsk() {
    if(!(sellLimitOrder.empty()) && !pegSellOrders.empty()) {
        if(pegSellOrders[1]->getPrice() != getBestAsk()) {
            double bestAsk = getBestAsk();
            for (auto& pegOrder : pegSellOrders) {
                pegOrder->setPrice(bestAsk);
            }
        }
    }
}

OrderManager::OrderManager() {
}

OrderManager::~OrderManager() {
    
    while (!buyLimitOrder.empty()) {
        buyLimitOrder.pop();
    }
    while (!sellLimitOrder.empty()) {
        sellLimitOrder.pop();
    }

    pegBuyOrders.clear();
    pegSellOrders.clear(); 
}

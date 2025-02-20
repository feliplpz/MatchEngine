#include "Match.h"

void Match::enableBookPrinting() { shouldPrintBook = true; }

void Match::disableBookPrinting() { shouldPrintBook = false; }

bool Match::isBookPrintingEnabled() { return shouldPrintBook; }

OrderManager &Match::getOrderManager() { return orderManager; }

void Match::printBook() {
    auto& buyLimits = orderManager.getBuyLimitOrder();
    auto& buyPegs = orderManager.getPegBuyOrders();
    auto& sellLimits = orderManager.getSellLimitOrder();
    auto& sellPegs = orderManager.getPegSellOrders();

    cout << "\n=== LIVRO DE ORDENS - " << name << " === " << endl;

    // Exibir ordens de venda (Ask)
    cout << "Ordens de Venda (Ask Orders):" << endl;
    priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> mergedSellOrders = sellLimits;
    for (auto order : sellPegs) {
        mergedSellOrders.emplace(order);
    }

    while (!mergedSellOrders.empty()) {
        OrderPassive* order = mergedSellOrders.top();
        cout << order->getQuantity() << " @ " << order->getPrice() 
             << (order->getType() == "peg" ? " (PEG)" : "") << endl;
        mergedSellOrders.pop();
    }

    cout << "-----------------------------" << endl;

    // Exibir ordens de compra (Bid)
    cout << "Ordens de Compra (Bid Orders):" << endl;
    priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> mergedBuyOrders = buyLimits;
    for (auto order : buyPegs) {
        mergedBuyOrders.emplace(order);
    }

    while (!mergedBuyOrders.empty()) {
        OrderPassive* order = mergedBuyOrders.top();
        cout << order->getQuantity() << " @ " << order->getPrice() 
             << (order->getType() == "peg" ? " (PEG)" : "") << endl;
        mergedBuyOrders.pop();
    }

    cout << "========================\n" << endl;
}

void Match::tradeActiveOrder(MarketOrder *market) {

    if (market->getSide() == "sell") {
        auto& pegVector = orderManager.getPegBuyOrders();
        auto& buyQueue = orderManager.getBuyLimitOrder(); 

        priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> mergedQueue = buyQueue;

        for (auto order : pegVector) {
            mergedQueue.emplace(order);
        }

        // Loop enquanto houver quantidade no market e ordens para processar
        while (market->getQuantity() > 0 && !mergedQueue.empty()) { 
            OrderPassive* topOrder = mergedQueue.top(); 

            double tradeQty = min(market->getQuantity(), topOrder->getQuantity());

            cout << "Trade, price: " << topOrder->getPrice() << ", qty: " << tradeQty << endl;

            // Caso a quantidade da ordem passiva tenha sido completamente consumida
            if (tradeQty == topOrder->getQuantity()) {  
                mergedQueue.pop();
                orderManager.executeOrder(topOrder->getId());
                orderManager.updatePegBid(); 


                market->setQuantity(market->getQuantity() - tradeQty);  // Atualizar a quantidade de market
            }
            // Caso a quantidade de market tenha sido completamente consumida
            else if (tradeQty == market->getQuantity()) {
                // Subtrair a quantidade da ordem passiva
                topOrder->setQuantity(topOrder->getQuantity() - tradeQty);
                market->setQuantity(0);  // A ordem market foi completamente consumida
            } 
        }
    }
    else {
        auto& sellPeg = orderManager.getPegSellOrders();
        auto& sellLimit = orderManager.getSellLimitOrder();

        priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> mergedQueue = sellLimit;

        for (auto order : sellPeg) {
            mergedQueue.emplace(order);
        }

        while (market->getQuantity() > 0 && !mergedQueue.empty()) { 
            OrderPassive* topOrder = mergedQueue.top();
            
        
            double tradeQty = min(market->getQuantity(), topOrder->getQuantity());

            cout << "Trade, price: " << topOrder->getPrice() << ", qty: " << tradeQty << endl;

      
            if (tradeQty == topOrder->getQuantity()) {  
                mergedQueue.pop();
                orderManager.executeOrder(topOrder->getId());
                orderManager.updatePegAsk(); 

                market->setQuantity(market->getQuantity() - tradeQty);  
            }
        
            else if (tradeQty == market->getQuantity()) {
             topOrder->setQuantity(topOrder->getQuantity() - tradeQty);
             market->setQuantity(0);  // A ordem market foi completamente consumida

        } 
        }

    }
 }




void Match::tradePassiveOrder(OrderPassive *orderPassive) {
 if(orderPassive->getType() == "peg"){ throw new logic_error("Só limits vão fechar negócio como ativas ao serem criadas"); }
    if (orderPassive->getSide() == "sell") {
        auto& buyPeg = orderManager.getPegBuyOrders();
        auto& buyLimit = orderManager.getBuyLimitOrder(); 

        priority_queue<OrderPassive*, vector<OrderPassive*>, BuyComparator> mergedQueue = buyLimit;

        for (auto order : buyPeg) {
            mergedQueue.emplace(order);
        }
        while(!mergedQueue.empty() && (orderPassive->getQuantity() > 0)) {
            OrderPassive *topOrder = mergedQueue.top();
                mergedQueue.pop(); 
            if(mergedQueue.top()->getPrice() == orderPassive->getPrice()) {
                double tradeQty = min(topOrder->getQuantity(), orderPassive->getQuantity());

                cout << "Trade, price: " << topOrder->getPrice() << ", qty: " << tradeQty << endl;
                
                if (tradeQty == topOrder->getQuantity()) {  
                    orderManager.executeOrder(topOrder->getId());
                    orderManager.updatePegBid(); 
                    orderPassive->setQuantity(orderPassive->getQuantity() - tradeQty);  
                }
                if (tradeQty == orderPassive->getQuantity()) {
                    topOrder->setQuantity(topOrder->getQuantity() - tradeQty);
                    orderPassive->setQuantity(0); 
                    orderManager.executeOrder(orderPassive->getId());
                
                } 
            }
            
        }
       
   }else {
        auto& sellPeg = orderManager.getPegSellOrders();
        auto& sellLimit = orderManager.getSellLimitOrder(); 

        priority_queue<OrderPassive*, vector<OrderPassive*>, SellComparator> mergedQueue = sellLimit;

        for (auto order : sellPeg) {
            mergedQueue.emplace(order);
        }
        
        while(!mergedQueue.empty() && (orderPassive->getQuantity() > 0)) {
            OrderPassive *topOrder = mergedQueue.top();
            mergedQueue.pop(); 
            if(mergedQueue.top()->getPrice() == orderPassive->getPrice()) {
                double tradeQty = min(topOrder->getQuantity(), orderPassive->getQuantity());
                
                cout << "Trade, price: " << topOrder->getPrice() << ", qty: " << tradeQty << endl;
                
                if (tradeQty == topOrder->getQuantity()) {  
                    orderManager.updatePegAsk(); 
                    orderPassive->setQuantity(orderPassive->getQuantity() - tradeQty);  
                    orderManager.executeOrder(topOrder->getId());
                    
                    
                } else if (tradeQty == orderPassive->getQuantity()) {
                    topOrder->setQuantity(topOrder->getQuantity() - tradeQty);
                    orderPassive->setQuantity(0); 
                    orderManager.executeOrder(orderPassive->getId());
                } 
            }

        }
        
    }   

}


Match::Match(string name): name(name){
}

Match::~Match() {
}

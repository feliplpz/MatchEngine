#ifndef MARKETORDER_H
#define MARKETORDER_H

#include <string>
#include <iostream>

using namespace std;

class MarketOrder {
protected:
   const string side;
   double quantity;
public:
    MarketOrder(const string side, double quantity);
    void setQuantity(double quantity);
    string getSide();
    double getQuantity();
    virtual ~MarketOrder();
};

#endif
#ifndef ORDERPASSIVE_H
#define ORDERPASSIVE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <unordered_map>

using namespace std;

class OrderPassive {
protected:
static int nextId; 
static unordered_map<string, OrderPassive*> activeOrders; 
string type;
int numId;
string id;
string subtype;
string side;
double quantity;
double price;

public:
    static OrderPassive* cancelOrder(const string& id);
    static OrderPassive* findOrder(const string& id);
    static OrderPassive* executeOrder(const string& id);
    void setQuantity(double quantity);
    double getPrice();
    void setPrice(double price);
    double getQuantity();
    int getNumId();
    string getType();
    string getSubtype();
    string getId();
    string getSide();
    OrderPassive(const string& side, double price, double quantity);
    OrderPassive(const string& side, const string& subtype, double quantity);
    virtual ~OrderPassive();
};

#endif

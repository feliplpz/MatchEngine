#include "OrderPassive.h"

unordered_map<string, OrderPassive*> OrderPassive::activeOrders;
int OrderPassive::nextId = 1;

OrderPassive* OrderPassive::cancelOrder(const string &id) {
    auto it = activeOrders.find(id);
    if (it != activeOrders.end()) {
        OrderPassive* orderToCancel = it->second;  
        activeOrders.erase(it);  
        cout << "Order cancelled "<< endl;
        return orderToCancel;  
    } else {
        throw invalid_argument("Order ID not found");
    }
}

OrderPassive *OrderPassive::findOrder(const string &id) {
    auto it = activeOrders.find(id);
    if (it != activeOrders.end()) {
        OrderPassive* orderToChange = it->second;    
        return orderToChange;  
    } else {
        throw invalid_argument("Order ID not found");
    }
}

OrderPassive *OrderPassive::executeOrder(const string &id) {
    auto it = activeOrders.find(id);
    if (it != activeOrders.end()) {
        OrderPassive* orderToExecute = it->second;  
        activeOrders.erase(it);  
        return orderToExecute;  
    } else {
        throw invalid_argument("Order ID not found");
    }
}


void OrderPassive::setQuantity(double quantity) {
    if(quantity < 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    this->quantity = quantity;
}

double OrderPassive::getPrice() {
    return price;
}

void OrderPassive::setPrice(double price) {
    if(price <= 0) { throw new invalid_argument("Price must be a real positive value"); }
    this->price = price;
}

double OrderPassive::getQuantity() {
    
    return quantity;
}

string OrderPassive::getType() {
    return type;
}

string OrderPassive::getSubtype() {
 if(subtype != "bid" && subtype != "ask"){ throw new logic_error("Subtype only exist for peg orders"); }
    return subtype;
}

string OrderPassive::getId() {
    return id;
}

string OrderPassive::getSide() {
    return side;
}

OrderPassive::OrderPassive(const string &side, double price, double quantity) : side(side), price(price), quantity(quantity), type("limit") {
    if(quantity <= 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    if(price <= 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    if(side != "buy" && side != "sell") { throw new invalid_argument("Side must be buy or sell"); }
    id = "identificador_" + to_string(nextId);
        activeOrders[id] = this;
    cout << "Order created: " << side << " " << quantity << " @ " << price << " " << id << endl;
    nextId++; 
}


OrderPassive::OrderPassive(const string &side, const string &subtype, double quantity) : side(side), subtype(subtype), quantity(quantity), type("peg"){
    if(quantity <= 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    if(side != "buy" && side != "sell") { throw new invalid_argument("Side must be buy or sell"); }
    if(subtype != "bid" && subtype != "ask") { throw new invalid_argument("Subtype must be bid or ask"); } 
    id = "identificador_" + to_string(nextId);
        activeOrders[id] = this; 
    cout << "Order created: Peg " << side << " " << subtype << " " << quantity  << " " << id << endl;
    nextId++;
}

OrderPassive::~OrderPassive() {  
    if (activeOrders.find(id) != activeOrders.end()) {  
    activeOrders.erase(id);
    } 
}

#include "MarketOrder.h"

MarketOrder::MarketOrder(const string side, double quantity): side(side), quantity(quantity) {
    if(quantity <= 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    if(side != "buy" && side != "sell") { throw new invalid_argument("Side must be buy or sell"); }
    cout << "Order created: market " << side << " " << quantity <<  endl;
}

void MarketOrder::setQuantity(double quantity) {
    if(quantity < 0) { throw new invalid_argument("Quantity must be a real positive value"); }
    this->quantity = quantity;
}

string MarketOrder::getSide()
{
    return side;
}

double MarketOrder::getQuantity() {
    return quantity;
}

MarketOrder::~MarketOrder() {}

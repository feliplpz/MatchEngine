#include <iostream>
#include <string>
#include <sstream>
#include "Match.h"

using namespace std;

void inicioDaEngine() {
    cout << "\n### COMANDOS DISPONIVEIS ###\n";
    
    cout << "\n1. Criar ordem limitada (Limit Order):\n";
    cout << "   >>> limit buy <preco> <quantidade>\n";
    cout << "   >>> limit sell <preco> <quantidade>\n";

    cout << "\n2. Criar ordem de mercado (Market Order):\n";
    cout << "   >>> market buy <quantidade>\n";
    cout << "   >>> market sell <quantidade>\n";

    cout << "\n3. Criar ordem peg (Peg Order):\n";
    cout << "   >>> peg bid buy <quantidade>\n";
    cout << "   >>> peg ask sell <quantidade>\n";

    cout << "\n4. Cancelar uma ordem:\n";
    cout << "   >>> cancel order <ID>\n";

    cout << "\n5. Alterar uma ordem existente:\n";
    cout << "   >>> change order <ID> <novo_preco> <nova_quantidade>\n";

    cout << "\n6. Controle de impressao do livro de ordens:\n";
    cout << "   >>> print book  (ativa a impressao automatica do livro de ordens)\n";
    cout << "   >>> print stop  (desativa a impressao automatica do livro de ordens)\n";

    cout << "\n7. Sair da engine:\n";
    cout << "   >>> exit\n";

    cout << "###################################\n";
}

void parseInput(string& input, Match& engine) {
    istringstream ss(input);
    string command, subcommand, side, subtype, id, trash;
    double quantity, price;

    ss >> command;

    if (command == "cancel") {
        ss >> trash >> id;
        engine.getOrderManager().cancelOrder(id);
    } 
    else if (command == "limit") {
        ss >> side >> price >> quantity;
        OrderPassive* order = new OrderPassive(side, price, quantity);
        engine.tradePassiveOrder(order);
        if(side == "buy" && !engine.getOrderManager().getPegBuyOrders().empty()) {
            engine.getOrderManager().updatePegBid();
        } else if (side == "sell" && !engine.getOrderManager().getPegSellOrders().empty()) {
            engine.getOrderManager().updatePegAsk();
        }
        engine.getOrderManager().addlimitOrder(order);
    } 
    else if (command == "market") {
        ss >> side >> quantity;
        MarketOrder* order = new MarketOrder(side, quantity);
        engine.tradeActiveOrder(order);
        if(side == "buy") {
            engine.getOrderManager().updatePegAsk();
        } else {
            engine.getOrderManager().updatePegBid();
        }
        delete order;
    } 
    else if (command == "peg") {
        ss >> subtype >> side >> quantity;
        OrderPassive* order = new OrderPassive(side, subtype, quantity);
        engine.getOrderManager().addpegOrder(order);
    } 
    else if (command == "change") {
        ss >> trash >> id >> price >> quantity;
        engine.getOrderManager().changeOrder(id, price, quantity);
    } 
    else if (command == "print") {
        ss >> subcommand;
        if (subcommand == "book") {
            engine.enableBookPrinting();
            engine.printBook();
            cout << ">>> Impressao automatica ativada.\n";
        } else if (subcommand == "stop") {
            engine.disableBookPrinting();
            cout << ">>> Impressao automatica desativada.\n";
        }
        return; 
    }

    if (engine.isBookPrintingEnabled()) {
        engine.printBook();
    }
}

int main() {
    Match matchEngine("PETR4");
    string input;

    cout << "Digite os comandos que dejesa\n";
    inicioDaEngine(); 

    while (true) {
        try {
            cout << ">>> ";
            getline(cin, input);

            if (input == "exit") {
                break;
            }

            parseInput(input, matchEngine);  
        } catch (exception *e) {
            cerr << "Erro: " << e->what() << endl;
            delete e;
        }
    }

    return 0;
}

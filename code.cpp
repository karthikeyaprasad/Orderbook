#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>

enum OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    int quantity;
    double price;

    Order(int id, OrderType type, int quantity, double price)
        : id(id), type(type), quantity(quantity), price(price) {}
};

struct CompareBuyOrders {
    bool operator()(Order const& a, Order const& b) {
        return a.price < b.price;
    }
};

struct CompareSellOrders {
    bool operator()(Order const& a, Order const& b) {
        return a.price > b.price;
    }
};

class OrderBook {
public:
    void addOrder(Order order);
    void matchOrders();
    void printOrderBook();

private:
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> sellOrders;
    int orderIdCounter = 0;
};

void OrderBook::addOrder(Order order) {
    if (order.type == BUY) {
        buyOrders.push(order);
    } else {
        sellOrders.push(order);
    }
    matchOrders();
}

void OrderBook::matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
        Order buyOrder = buyOrders.top();
        Order sellOrder = sellOrders.top();

        if (buyOrder.price >= sellOrder.price) {
            int quantity = std::min(buyOrder.quantity, sellOrder.quantity);

            std::cout << "Trade executed: "
                      << "Buy Order ID: " << buyOrder.id
                      << " Sell Order ID: " << sellOrder.id
                      << " Quantity: " << quantity
                      << " Price: " << sellOrder.price << std::endl;

            buyOrders.pop();
            sellOrders.pop();

            if (buyOrder.quantity > quantity) {
                buyOrder.quantity -= quantity;
                buyOrders.push(buyOrder);
            }

            if (sellOrder.quantity > quantity) {
                sellOrder.quantity -= quantity;
                sellOrders.push(sellOrder);
            }
        } else {
            break;
        }
    }
}

void OrderBook::printOrderBook() {
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrders> tempBuyOrders = buyOrders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrders> tempSellOrders = sellOrders;

    std::cout << "Buy Orders:" << std::endl;
    while (!tempBuyOrders.empty()) {
        Order order = tempBuyOrders.top();
        std::cout << "ID: " << order.id << " Quantity: " << order.quantity << " Price: " << order.price << std::endl;
        tempBuyOrders.pop();
    }

    std::cout << "Sell Orders:" << std::endl;
    while (!tempSellOrders.empty()) {
        Order order = tempSellOrders.top();
        std::cout << "ID: " << order.id << " Quantity: " << order.quantity << " Price: " << order.price << std::endl;
        tempSellOrders.pop();
    }
}

int main() {
    OrderBook orderBook;

    orderBook.addOrder(Order(1, BUY, 10, 100.0));
    orderBook.addOrder(Order(2, SELL, 5, 99.0));
    orderBook.addOrder(Order(3, BUY, 15, 101.0));
    orderBook.addOrder(Order(4, SELL, 10, 100.0));

    orderBook.printOrderBook();

    return 0;
}

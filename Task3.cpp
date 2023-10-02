#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>

enum {
	CHICKEN,
	SOUP,
	STAKE,
	SALAD,
	SUSHI
};

std::mutex order_lock;
std::mutex kitchen_lock;

void printOrder(int order) {
    if (order == CHICKEN) {
        std::cout << "Chicken ordered"<< std::endl;
    }
    else if (order == SOUP) {
        std::cout << "Soup ordered" << std::endl;
    }
    else if (order == STAKE) {
        std::cout << "Stake ordered" << std::endl;
    }
    else if (order == SALAD) {
        std::cout << "Salad ordered" << std::endl;
    }
    else if (order == SUSHI) {
        std::cout << "Sushi ordered" << std::endl;
    }
}

void printCook(int order) {
    if (order == CHICKEN) {
        std::cout << "Chicken cooked" << std::endl;
    }
    else if (order == SOUP) {
        std::cout << "Soup cooked" << std::endl;
    }
    else if (order == STAKE) {
        std::cout << "Stake cooked" << std::endl;
    }
    else if (order == SALAD) {
        std::cout << "Salad cooked" << std::endl;
    }
    else if (order == SUSHI) {
        std::cout << "Sushi cooked" << std::endl;
    }
}

void printDelivery(int order) {
    if (order == CHICKEN) {
        std::cout << "Chicken is being delivered" << std::endl;
    }
    else if (order == SOUP) {
        std::cout << "Soup is being delivered" << std::endl;
    }
    else if (order == STAKE) {
        std::cout << "Stake is being delivered" << std::endl;
    }
    else if (order == SALAD) {
        std::cout << "Salad is being delivered" << std::endl;
    }
    else if (order == SUSHI) {
        std::cout << "Sushi is being delivered" << std::endl;
    }
}

void order(std::vector<int> *orders) {
    int order;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 6 + 5));
        order = std::rand() % 5;
        order_lock.lock();
        orders->emplace(orders->begin(), order);
        order_lock.unlock();
        printOrder(order);
    }
}

void cook(std::vector<int> *orders, std::vector<int> *cookedOrders) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 11 + 5));
        kitchen_lock.lock();
        cookedOrders->emplace(cookedOrders->begin(), orders->back());
        kitchen_lock.unlock();
        printCook(orders->back());
        order_lock.lock();
        orders->pop_back();
        order_lock.unlock();
    }
}

void delivery(std::vector<int> *cookedOrders) {
    int counter = 0;
    while (counter < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        for (int i = 0; i < cookedOrders->size(); ++i) {
            printDelivery(cookedOrders->back());
            kitchen_lock.lock();
            cookedOrders->pop_back();
            kitchen_lock.unlock();
            counter++;
        }
    }
}

int main() {
    std::srand(std::time(nullptr));
    std::vector<int> online_orders;
    std::vector<int> cooked_orders;
    std::thread thOrders(order, &online_orders);
    thOrders.detach();
    std::thread thKitchen(cook, &online_orders, &cooked_orders);
    thKitchen.detach();
    std::thread thDelivery(delivery, &cooked_orders);
    thDelivery.join();
    std::cout << "Finish";
}
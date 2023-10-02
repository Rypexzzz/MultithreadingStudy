#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

struct Train {
	char name = ' ';
	int arrivalTime = 0;
};

std::mutex availability_lock;

void trainDeparture(Train train) {
	std::this_thread::sleep_for(std::chrono::seconds(train.arrivalTime));
	if (!availability_lock.try_lock()) {
		std::cout << "Train " << train.name << " should wait. Entrance is busy" << std::endl;
		availability_lock.lock();
	}
	std::cout << "Train " << train.name << " arrived" << std::endl;
	std::cout << "Input depart to continue" << std::endl;
	std::string input;
	std::cin >> input;
	while (input != "depart") {
		std::cout << "Wrong input, try again" << std::endl;
		std::cin >> input;
	}
	availability_lock.unlock();
}

int main() {
	char names[3] = { 'A', 'B', 'C' };
	Train trains[3];
	for (int i = 0; i < 3; i++) {
		int buf;
		std::cout << "Input arrival time of train " << names[i] << std::endl;
		std::cin >> buf;
		trains[i] = { names[i], buf };
	}
	std::vector<std::thread> trainThreads;
	trainThreads.reserve(3);
	for (int i = 0; i < 3; i++) {
		trainThreads.emplace_back(trainDeparture, trains[i]);
	}
	for (int i = 0; i < 3; i++) {
		trainThreads[i].join();
	}
}
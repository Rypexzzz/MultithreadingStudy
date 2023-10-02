#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

std::vector<std::string> results;
std::mutex results_lock;

void swim(std::string name, double speed) {
	double distance = 0;
	std::cout << name << " started swimming." << std::endl;
	while (distance < 100) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << name << " swam " << distance << " metres" << std::endl;
		distance += speed;
	}
	std::cout << name << " finished." << std::endl;
	results_lock.lock();
	results.push_back(name);
	results_lock.unlock();
}

 int main() {
	int n = 2;
	std::vector <std::thread> swimmers;
	std::vector <std::string> names(n);
	std::vector <double> speeds(n);
	for (int i = 0; i < n; i++) {
		std::cout << "Input name and speed of a swimmer:" << std::endl;
		std::cin >> names[i] >> speeds[i];
	}
	for (int i = 0; i < n; i++) {
		swimmers.emplace_back(swim, names[i], speeds[i]);
	}
	for (int i = 0; i < n; i++) {
		swimmers[i].join();
	}
	for (int i = 0; i < n; i++) {
		std::cout << i+1 << ". " << results[i] << std::endl;
	}
}
#include "r.hpp"
#include <iostream>
#include <string>

int main() {
	std::cout << "hello world" << std::endl;
	auto integerB = make(0);
	auto stringB  = make<std::string>("hello there");

	subscribe<int>(integerB, [](const int& i) { std::cout << i << std::endl; });
	subscribe<std::string>(stringB, [](const std::string& s) { std::cout << s << std::endl; });

	for (int i = 0; i != 10; ++i) {
		next<int>(integerB, i);
		next<std::string>(stringB, stringB->value() + "z");
	}
	return 0;
}

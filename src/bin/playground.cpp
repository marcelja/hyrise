#include <iostream>

#include "../lib/storage/value_vector.hpp"


void benchmark() {
	opossum::ValueVector<std::string> a;
	opossum::ValueVector<opossum::FixedString> b(10);
	std::vector<std::string> c;

	std::string insert_me{"blablabla"};
	size_t inserts = 10000000;
	size_t searches = 10000;
	for(int run = 0; run < 2; ++run) {
		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < inserts; ++i) a.push_back(insert_me);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "inserting " << inserts << " values into opossum::ValueVector<std::string>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}

		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < inserts; ++i) b.push_back(opossum::FixedString(insert_me));
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "inserting " << inserts << " values into opossum::ValueVector<opossum::FixedString>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}

		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < inserts; ++i) c.push_back(insert_me);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "inserting " << inserts << " values into std::vector<std::string>:      " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}
	}
	for(int run = 0; run < 2; ++run) {
		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < searches; ++i) std::lower_bound(a.begin(), a.end(), insert_me);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "binary search for " << searches << " values in opossum::ValueVector<std::string>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}

		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < searches; ++i) std::lower_bound(b.begin(), b.end(), opossum::FixedString(insert_me));
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "binary search for " << searches << " values in opossum::ValueVector<opossum::FixedString>: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}

		{
			auto t1 = std::chrono::high_resolution_clock::now();
			for(size_t i = 0; i < searches; ++i) std::lower_bound(c.begin(), c.end(), insert_me);
			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "binary search for " << searches << " values in std::vector<std::string>:      " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
		}
	}
}

int main () {
	// opossum::ValueVector<std::string> a;
	// opossum::ValueVector<opossum::FixedString> b(10);

	// for (auto s : {"hello", "world", "areallylongstring", "modifyme"}) {
	// 	a.push_back(std::string(s));
	// 	b.push_back(std::string(s));
	// }

	// std::cout << "a[3] == " << a[3] << std::endl;
	// std::cout << "b[3] == " << b[3] << std::endl;

	// a[3] = "modified";
	// b[3] = std::string("modified");

	// std::cout << "std::string: ";
	// for (const auto &s : a) std::cout << s << " | ";
	// std::cout << std::endl;

	// std::cout << "opossum::FixedString: ";
	// for (const auto &s : b) std::cout << s << " | ";
	// std::cout << std::endl;

	benchmark();
}

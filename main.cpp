#include <string>
#include <iostream>
#include <vector>
#include <limits>

#include "wmml.h"





void r (std::string& file) {
	wmml readfile(file);
	std::vector<std::string> s(readfile.size());
	while (readfile.read(s)) {
		for(std::string out : s)
			std::cout << out << " ";
		std::cout << std::endl;
	}
}

void c (std::string& file) {
	int size;
	std::cout << "Enter the grid size: " << std::endl;
	std::cin >> size;
	wmml editfile(file, size);
	std::vector<std::string> c(size);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	for(int num = 0; num != size; ++num) {
		std::string tmp;
		int lnum = num;
		std::cout << "\033[2J\033[1;1H";
		std::cout << "[" << ++lnum << "/" << size << "] Enter the string: " << std::endl;
		std::getline(std::cin, tmp);
		c[num] = tmp;
	}
	editfile.add(c);
}

void o (std::string& file) {
	int index;
	int field;
	std::string str;
	r(file);
	std::cout << "" << std::endl;
	std::cout << "Enter the object index:" << std::endl;
	std::cin >> index;
	std::cout << "Enter the field index:" << std::endl;
	std::cin >> field;
	std::cout << "Enter the string:" << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, str);
	wmml rewritableFile(file);
	rewritableFile.overwriting(index, field, str);
}




int main(int argc, char* argv[]) {
	char modecase;
	std::string file = argv[1];
	for (;;) {
		std::cin >> modecase;
		switch(modecase){
			case 'r':
				r(file);
				break;
			case 'c':
				c(file);
				break;
			case 'o':
				o(file);
				break;
			case 'd':
				remove(file.c_str());
				return 0;
				break;
		}
	}
	return 0;
}
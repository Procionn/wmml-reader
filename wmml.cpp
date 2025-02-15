#include "wmml.h"

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

wmml::wmml (std::string& file) : targetFile(file.c_str(), std::ios::binary | std::ios::in | std::ios::out) {
    if (!targetFile.is_open()) {
        std::cerr << "file is not found" << std::endl;
        std::abort();
    }
    else {
        targetFile.seekp(0);
        targetFile.read(reinterpret_cast<char*>(&ObjectsInThread), sizeof(unsigned short int));
    }
    filename = file;
}
wmml::wmml (std::string& file, int OIT) : targetFile(file.c_str(), std::ios::binary | std::ios::in | std::ios::out) {
    if (!targetFile.is_open()) {
        ObjectsInThread = OIT;
        std::ofstream File(file.c_str(), std::ios::binary);
        File.close();
        targetFile.open(file.c_str(), std::ios::binary | std::ios::in | std::ios::out);
        targetFile.seekp(0);
        targetFile.write(reinterpret_cast<const char*>(&ObjectsInThread), sizeof(ObjectsInThread));
    }
    else {
        targetFile.seekp(0);
        targetFile.read(reinterpret_cast<char*>(&ObjectsInThread), sizeof(unsigned short int));
        std::cerr << "file is exists" << std::endl;
        std::abort();
    }
    filename = file;
}

wmml::~wmml() {
    targetFile.close();
}

unsigned short int wmml::size() {
	return ObjectsInThread;
}


bool wmml::read (std::vector<std::string>& out) {
    if (!targetFile.is_open()) std::cerr << "Couldn't open the file for reading!" << std::endl;
    unsigned short int stringSize;
    if (targetFile.eof()) return false;
    else {
        for (int num = 0; num != ObjectsInThread; ++num ) {
            targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
            if (targetFile.eof()) return false;
            std::string str(stringSize, '\0');
            targetFile.read(&str[0], stringSize);
            out[num] = str;
        }
    }
    return true;
}

bool wmml::read () {
    if (!targetFile.is_open()) std::cerr << "Couldn't open the file for reading!" << std::endl;
    unsigned short int stringSize;
    if (targetFile.eof()) return false;
    else {
        for (int num = 0; num != ObjectsInThread; ++num ) {
            targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
            if (targetFile.eof()) return false;
            std::string str(stringSize, '\0');
            targetFile.read(&str[0], stringSize);
        }
    }
    return true;
}



void wmml::add (std::vector<std::string>& in) {
    if (targetFile.eof()) targetFile.clear();
    targetFile.seekp(0, std::ios::end);
    if (!targetFile.is_open()) std::cerr << "Не удалось открыть файл для дозаписи!" << std::endl;
    for (std::string inputString : in) {
        unsigned short int stringSize = inputString.size();
        targetFile.write(reinterpret_cast<const char*>(&stringSize), sizeof(stringSize));
        targetFile.write(inputString.data(), stringSize);
    }
}


void wmml::reset() {
    targetFile.clear();
    targetFile.seekp(sizeof(unsigned short int));
}

int wmml::sizeRequest() {
    int counter = 0;
    while (read())
        ++counter;
    return counter;
}


void wmml::overwriting (int tag, int field, std::string newStr) {
    if (field > ObjectsInThread) 
        abcerr("this fields does not exist in object!");
    for (;tag != 0; --tag) {
        if (!read()) 
            abcerr("this object does not exist in file!");
    }
    int mark1 = targetFile.tellp();
    unsigned short int stringSize;
    for (; field != 0; --field) {
        targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
        if (targetFile.eof()) 
            abcerr("the end of the file is reached before the end of the cycle!");
        std::string str(stringSize, '\0');
        targetFile.read(&str[0], stringSize);
    }
    int mark2 = targetFile.tellp();
    if (targetFile.eof()) 
        abcerr("the end of the file is reached before the end of the cycle!");
    targetFile.read(reinterpret_cast<char*>(&stringSize), sizeof(stringSize));
    if (stringSize == newStr.size()) {
        targetFile.write(newStr.data(), stringSize);
    }
    else {
        std::cout << "string: " << newStr << std::endl;
        std::cout << stringSize << "->" << newStr.size() << std::endl;
        abcerr("the size of the new value does not match the size of the field being overwritten!");
    }
}
#if 0
void wmml::replace (int tag, std::vector<std::string>& in) {
    if (fields > ObjectsInThread) 
        abcerr("this fields does not exist in object!");
    for (; tag != 0; --tag) {
        if (!read()) 
            abcerr("this object does not exist in file!");
    }
    int mark1 = targetFile.tellp();
}
#endif

void wmml::abcerr (std::string error) {
    std::cerr << error << std::endl;
    abort();
}

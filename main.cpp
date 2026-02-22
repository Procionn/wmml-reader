/*
 *  Copyright (C) 2026 Procion ByProcion@gmail.com
 *
 *  This file is part of WMML reader.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 */
#include <filesystem>
#include <iostream>
#include <ranges>
#include <stdint.h>
#include <string>
#include <vector>
#include <wmml.h>

template <typename T> void print (T value, uint maxsize) {
    std::cout << std::setw(maxsize) << value;
}

template <> void print<char>(char value, uint maxsize) {
    std::cout << std::setw(maxsize) << static_cast<int>(value);
}
template <> void print<signed char>(signed char value, uint maxsize) {
    std::cout << std::setw(maxsize) << static_cast<int>(value);
}
template <> void print<wchar_t>(wchar_t value, uint maxsize) {
    std::cout << "printing wchar not supported on linux build";
}


void write (const std::vector<wmml::variant>& data, const std::vector<uint>& header,
            const int& counter) {
    auto size = data.size();
    std::cout << counter << ")  ";
    for (uint i = 0; i != size; i++) {
        auto hedersize = header[i];
        std::visit([hedersize] (auto value) { print(value, hedersize); }, data[i]);
        std::cout << " ";
    }
    std::cout << "\n";
}


template <typename T> uint get_size (T value) { return std::to_string(value).size(); }
template <> uint get_size<std::string>(std::string value) { return value.size() + 2; }
template <> uint get_size<char>(char value) { return 4; }
template <> uint get_size<u_char>(u_char value) { return 3; }
template <> uint get_size<wchar_t>(wchar_t value) { return 6; }
template <> uint get_size<bool>(bool value) { return 3; }

template <typename T> void print_type (T value) { std::cout << "unknown type"; }
template <> void print_type<int>(int) { std::cout << "int"; }
template <> void print_type<unsigned int>(unsigned int) { std::cout << "unsigned int"; }
template <> void print_type<long>(long) { std::cout << "long int"; }
template <> void print_type<unsigned long>(unsigned long) { std::cout << "unsigned long int"; }
template <> void print_type<long long>(long long) { std::cout << "int64_t"; }
template <> void print_type<unsigned long long>(unsigned long long) { std::cout << "uint64_t"; }
template <> void print_type<short>(short) { std::cout << "short int"; }
template <> void print_type<unsigned short>(unsigned short) { std::cout << "unsigned short int"; }
template <> void print_type<char>(char) { std::cout << "char"; }
template <> void print_type<signed char>(signed char) { std::cout << "signed char"; }
template <> void print_type<unsigned char>(unsigned char) { std::cout << "unsigned char"; }
template <> void print_type<wchar_t>(wchar_t) { std::cout << "wchar_t"; }
template <> void print_type<std::string>(std::string) { std::cout << "std::string"; }
template <> void print_type<float>(float) { std::cout << "float"; }
template <> void print_type<double>(double) { std::cout << "double"; }
template <> void print_type<long double>(long double) { std::cout << "long double"; }
template <> void print_type<bool>(bool) { std::cout << "bool"; }


template <typename T> uint static_get_size (T value) {
    uint reg;
    std::visit([&reg] (auto ref) { reg = get_size(ref); }, value);
    return reg;
}


void signature (const std::vector<wmml::variant>& data, const int height, const int width) {
    std::cout << "File signature: height " << height << "  width " << width << "\n";
    for (auto entry : data) {
        std::visit(
            [] (auto value) {
                print_type(value);
                std::cout << " | ";
            },
            entry);
    }
    std::cout << "\n";
}


void read_file (const std::filesystem::path& path) {
    wmml file(path);
    std::vector<std::vector<wmml::variant>> array;
    std::vector<wmml::variant> v(file.width());
    array.reserve(file.height());
    while (file.read(v))
        array.emplace_back(v);

    std::vector<uint> header;
    header.reserve(file.width());
    for (int i = 0; i != file.width(); i++) {
        auto value = std::max_element(array.begin(), array.end(), [&i] (auto a, auto b) {
            return (static_get_size(a[i]) < static_get_size(b[i]));
        });
        header.emplace_back(static_get_size((*value)[i]));
    }
    signature(array[0], array.size(), file.width());
    std::cout << "\n";
    int index = 0;
    for (auto& entry : array) {
        write(entry, header, index);
        ++index;
    }
}


int main (int argc, char* argv[]) {
    if (argc >= 2) {
        std::filesystem::path filepath(argv[1]);
        read_file(filepath);
    }
    else
        std::cerr << "the file path has not been provided" << std::endl;
}

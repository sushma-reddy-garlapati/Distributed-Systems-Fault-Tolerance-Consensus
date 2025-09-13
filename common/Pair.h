#pragma once
#include <string>

struct Pair {
    std::string key;
    std::string value;

    Pair(const std::string& k, const std::string& v) : key(k), value(v) {}
};

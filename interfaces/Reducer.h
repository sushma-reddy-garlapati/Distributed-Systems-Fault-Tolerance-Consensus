#pragma once
#include <vector>
#include <string>

class Reducer {
public:
    virtual std::string reduce(const std::string& key, const std::vector<std::string>& values) = 0;
    virtual ~Reducer() = default;
};

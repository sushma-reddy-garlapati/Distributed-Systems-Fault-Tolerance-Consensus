#pragma once
#include <vector>
#include <string>
#include "../common/Pair.h"

class Mapper {
public:
    virtual std::vector<Pair> map(const std::string& key, const std::string& value) = 0;
    virtual ~Mapper() = default;
};

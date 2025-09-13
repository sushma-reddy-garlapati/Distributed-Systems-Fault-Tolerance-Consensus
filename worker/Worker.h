#pragma once
#include "../interfaces/Mapper.h"
#include "../common/Pair.h"
#include <vector>
#include <string>

class Worker {
public:
    Worker(Mapper* mapper);
    std::vector<Pair> execute(const std::string& key, const std::string& value);

private:
    Mapper* mapper_;
};

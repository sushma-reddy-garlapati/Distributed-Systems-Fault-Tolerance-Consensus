#pragma once
#include "../interfaces/Mapper.h"
#include <vector>
#include <string>
#include "../common/Pair.h"

class WordCountMapper : public Mapper {
public:
    std::vector<Pair> map(const std::string& key, const std::string& value) override;
};

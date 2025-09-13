#pragma once
#include "../interfaces/Reducer.h"
#include <string>
#include <vector>

class WordCountReducer : public Reducer {
public:
    std::string reduce(const std::string& key, const std::vector<std::string>& values) override;
};

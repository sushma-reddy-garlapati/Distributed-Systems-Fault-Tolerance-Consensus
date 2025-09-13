#pragma once
#include <vector>
#include <string>
#include "../implementations/WordCountMapper.h"
#include "../implementations/WordCountReducer.h"

class Master {
public:
    void run(const std::string& input);
};

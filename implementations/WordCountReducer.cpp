#include "WordCountReducer.h"
#include <sstream>

std::string WordCountReducer::reduce(const std::string& key, const std::vector<std::string>& values) {
    int count = 0;
    for (const auto& val : values) {
        count += std::stoi(val);
    }

    std::ostringstream result;
    result << key << " → " << count;
    return result.str();
}

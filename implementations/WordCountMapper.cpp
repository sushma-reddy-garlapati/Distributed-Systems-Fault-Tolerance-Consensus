#include "WordCountMapper.h"
#include <sstream>

std::vector<Pair> WordCountMapper::map(const std::string& key, const std::string& value) {
    std::vector<Pair> results;
    std::stringstream ss(value);
    std::string word;

    while (ss >> word) {
        results.emplace_back(word, "1");
    }

    return results;
}

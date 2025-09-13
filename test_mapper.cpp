#include "implementations/WordCountMapper.h"
#include <iostream>
#include <vector>

int main() {
    WordCountMapper mapper;

    std::string key = "chunk_0";
    std::string value = "Hello world hello";

    std::vector<Pair> output = mapper.map(key, value);

    std::cout << "Mapper Output:\n";
    for (const auto& kv : output) {
        std::cout << kv.key << " => " << kv.value << "\n";
    }

    return 0;
}

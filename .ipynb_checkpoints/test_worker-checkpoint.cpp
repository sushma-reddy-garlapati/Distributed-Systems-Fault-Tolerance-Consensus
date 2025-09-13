#include "worker/Worker.h"
#include "implementations/WordCountMapper.h"
#include <iostream>
#include <vector>

int main() {
    WordCountMapper* mapper = new WordCountMapper();
    Worker worker(mapper);

    std::string key = "chunk_0";
    std::string value = "hello world hello distributed systems";

    std::vector<Pair> result = worker.execute(key, value);

    std::cout << "Worker Output:\n";
    for (const auto& kv : result) {
        std::cout << kv.key << " => " << kv.value << "\n";
    }

    delete mapper; // Clean up if needed
    return 0;
}

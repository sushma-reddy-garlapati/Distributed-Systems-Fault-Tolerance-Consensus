#include "implementations/WordCountReducer.h"
#include <iostream>
#include <vector>

int main() {
    WordCountReducer reducer;

    std::string key = "hello";
    std::vector<std::string> values = {"1", "1", "1"};

    std::string result = reducer.reduce(key, values);

    std::cout << "Reducer Output:\n";
    std::cout << key << " => " << result << "\n";

    return 0;
}

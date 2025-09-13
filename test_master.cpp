#include "master/Master.h"
#include <iostream>

int main() {
    Master master;

    std::string input = "hello world hello distributed systems";
    master.run(input);

    std::cout << "Master pipeline executed.\n";
    return 0;
}

#include "SnapshotManager.h"
#include <fstream>
#include "../external/json.hpp"
#include <iostream>




using json = nlohmann::json;

void SnapshotManager::saveSnapshot(const std::vector<Pair>& data, const std::string& filename) {
    json snapshot;
    for (const auto& p : data) {
        snapshot.push_back({ {"key", p.key}, {"value", p.value} });
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << snapshot.dump(4);  // Pretty print
        file.close();
    }
}

std::vector<Pair> SnapshotManager::loadSnapshot(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Pair> data;


    if (file.is_open()) {
        std::cout << "Loading snapshot from " << filename << std::endl;
        json snapshot = json::parse(file);
        std::cout << "Snapshot content:\n" << snapshot.dump(4) << std::endl;
        for (const auto& item : snapshot) {
            data.emplace_back(item["key"], item["value"]);
        }
        file.close();
    }

    return data;
}

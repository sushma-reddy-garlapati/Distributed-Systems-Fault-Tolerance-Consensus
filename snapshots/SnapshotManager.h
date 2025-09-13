#pragma once
#include "../common/Pair.h"
#include <vector>
#include <string>

class SnapshotManager {
public:
    void saveSnapshot(const std::vector<Pair>& data, const std::string& filename);
    std::vector<Pair> loadSnapshot(const std::string& filename);
};

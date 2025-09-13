#include "Master.h"
#include "../worker/Worker.h"
#include "../implementations/WordCountMapper.h"
#include "../implementations/WordCountReducer.h"
#include "../snapshots/SnapshotManager.h"
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

void Master::run(const std::string& input) {
    WordCountMapper mapper;
    WordCountReducer reducer;
    SnapshotManager snapshotManager;
    std::vector<std::string> lines;
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }
    std::vector<Pair> intermediate;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i == 1) {
            std::ofstream faultLog("snapshots/fault_log.txt", std::ios::app);
            faultLog << "Skipped chunk " << i << std::endl;
            faultLog.close();
            continue;
        }
        Worker worker(&mapper);
        auto mapped = worker.execute(std::to_string(i), lines[i]);
        intermediate.insert(intermediate.end(), mapped.begin(), mapped.end());
    }
    snapshotManager.saveSnapshot(intermediate, "snapshots/snapshot_001.json");
    std::map<std::string, std::vector<std::string>> grouped;
    for (const auto& p : intermediate) {
        grouped[p.key].push_back(p.value);
    }
    std::ofstream outFile("output/result.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return;
    }
    for (const auto& entry : grouped) {
        std::string reduced = reducer.reduce(entry.first, entry.second);
        outFile << reduced << std::endl;
    }
    outFile.close();
}
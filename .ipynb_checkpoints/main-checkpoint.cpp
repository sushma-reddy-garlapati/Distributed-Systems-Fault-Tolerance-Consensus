#include "raft/RaftNode.h"
#include "worker/Worker.h"
#include "implementations/WordCountMapper.h"
#include "implementations/WordCountReducer.h"
#include "snapshots/SnapshotManager.h"
#include <vector>
#include <memory>
#include <iostream>
#include <chrono>

int main() {
    // Initialize Raft cluster
    std::vector<std::shared_ptr<RaftNode>> cluster;
    for (int i = 0; i < 3; ++i) {
        cluster.push_back(std::make_shared<RaftNode>(i));
    }

    // Initialize MapReduce components
    WordCountMapper* mapper = new WordCountMapper();
    WordCountReducer reducer;
    Worker worker(mapper);
    SnapshotManager snapshotManager;

    // Simulated input chunk
    std::string chunk = "hello world hello distributed systems";

    // Replicate logs to followers
    // if (cluster[0] && cluster[0]->getRole() == NodeRole::Leader) {
    //     auto startRep1 = std::chrono::high_resolution_clock::now();
    //     cluster[0]->replicateLogToFollower(cluster[1]);
    //     auto endRep1 = std::chrono::high_resolution_clock::now();
    //     auto durationRep1 = std::chrono::duration_cast<std::chrono::microseconds>(endRep1 - startRep1);
    //     std::cout << "Replication to Node 1 took " << durationRep1.count() << " µs\n";
    //     auto startRep2 = std::chrono::high_resolution_clock::now();
    //     cluster[0]->replicateLogToFollower(cluster[2]);
    //     auto endRep2 = std::chrono::high_resolution_clock::now();
    //     auto durationRep2 = std::chrono::duration_cast<std::chrono::microseconds>(endRep2 - startRep2);
    //     std::cout << "Replication to Node 2 took " << durationRep2.count() << " µs\n";
    // }


    // Node 0 starts election
    cluster[0]->startElection();
    for (int i = 1; i < 3; ++i) {
        cluster[i]->voteRequest(0, cluster[0]->getTerm());
    }

    // Node 0 becomes leader
    cluster[0]->receiveHeartbeat(cluster[0]->getTerm(), 0);
    for (int i = 1; i < 3; ++i) {
        cluster[i]->receiveHeartbeat(cluster[0]->getTerm(), 0);
    }
    cluster[0]->becomeLeader();
    if (cluster[0] && cluster[0]->getRole() == NodeRole::Leader) {
        auto startRep1 = std::chrono::high_resolution_clock::now();
        cluster[0]->replicateLogToFollower(cluster[1]);
        auto endRep1 = std::chrono::high_resolution_clock::now();
        auto durationRep1 = std::chrono::duration_cast<std::chrono::microseconds>(endRep1 - startRep1);
        std::cout << "Replication to Node 1 took " << durationRep1.count() << " µs\n";
        auto startRep2 = std::chrono::high_resolution_clock::now();
        cluster[0]->replicateLogToFollower(cluster[2]);
        auto endRep2 = std::chrono::high_resolution_clock::now();
        auto durationRep2 = std::chrono::duration_cast<std::chrono::microseconds>(endRep2 - startRep2);
        std::cout << "Replication to Node 2 took " << durationRep2.count() << " µs\n";
    }


    // Leader executes MapReduce task
    auto startMap = std::chrono::high_resolution_clock::now();
    std::vector<Pair> mapped = worker.execute("chunk_0", chunk);
    auto endMap = std::chrono::high_resolution_clock::now();
    auto durationMap = std::chrono::duration_cast<std::chrono::microseconds>(endMap - startMap);
    std::cout << "Mapping took " << durationMap.count() << " µs\n";
    
    cluster[0]->appendLog("Mapped chunk_0 with " + std::to_string(mapped.size()) + " pairs");
    
    // Reduce "hello" as example
    std::vector<std::string> helloValues = {"1", "1"};
    auto startReduce = std::chrono::high_resolution_clock::now();
    std::string reduced = reducer.reduce("hello", helloValues);
    auto endReduce = std::chrono::high_resolution_clock::now();
    auto durationReduce = std::chrono::duration_cast<std::chrono::microseconds>(endReduce - startReduce);
    std::cout << "Reducing 'hello' took " << durationReduce.count() << " µs\n";
    
    cluster[0]->appendLog("Reduced hello => " + reduced);
    std::cout << "Reduced hello => " << reduced << "\n";

    // // Leader executes MapReduce task
    // std::vector<Pair> mapped = worker.execute("chunk_0", chunk);
    // cluster[0]->appendLog("Mapped chunk_0 with " + std::to_string(mapped.size()) + " pairs");

    // // Reduce "hello" as example
    // std::vector<std::string> helloValues = {"1", "1"};
    // std::string reduced = reducer.reduce("hello", helloValues);
    // cluster[0]->appendLog("Reduced hello => " + reduced);
    // std::cout << "Reduced hello => " << reduced << "\n";


    // Replicate logs to followers
    if (cluster[0] && cluster[0]->getRole() == NodeRole::Leader) {
    cluster[0]->replicateLogToFollower(cluster[1]);
    cluster[0]->replicateLogToFollower(cluster[2]);
}

    std::cout << "\n--- Logs Before Leader Crash ---\n";
    for (int i = 0; i < 3; ++i) {
        if (cluster[i]) {
            cluster[i]->printLog();
        } else {
            std::cout << "Node " << i << " is offline.\n";
        }
    }

    // Simulate crash of Node 0
    std::cout << "\n--- Simulating Leader Crash (Node 0) ---\n";
    cluster[0] = nullptr;

    // Node 1 starts election
    cluster[1]->startElection();
    cluster[2]->voteRequest(1, cluster[1]->getTerm());

    // Node 1 becomes leader
    cluster[1]->receiveHeartbeat(cluster[1]->getTerm(), 1);
    cluster[2]->receiveHeartbeat(cluster[1]->getTerm(), 1);
    cluster[1]->becomeLeader();


    // Leader 1 appends recovery log
    cluster[1]->appendLog("Recovery initiated");

    // Re-replicate logs to Node 2
    cluster[1]->replicateLogToFollower(cluster[2]);

    // Snapshot after recovery
    std::vector<std::string> rawLog = cluster[1]->getLog();
    std::vector<Pair> snapshotData;
    for (const auto& entry : rawLog) {
        snapshotData.emplace_back("log_entry", entry);  // You can customize the key
    }
    auto startSnap = std::chrono::high_resolution_clock::now();
    snapshotManager.saveSnapshot(snapshotData, "Node_1.json");
    auto endSnap = std::chrono::high_resolution_clock::now();
    auto durationSnap = std::chrono::duration_cast<std::chrono::microseconds>(endSnap - startSnap);
    std::cout << "Snapshot saving took " << durationSnap.count() << " µs\n";
    std::cout << "Snapshot saved to Node_1.json with " << snapshotData.size() << " entries.\n";
    // snapshotManager.saveSnapshot(snapshotData, "Node_1.json");
    // std::cout << "Snapshot saved to Node_1.json with " << snapshotData.size() << " entries.\n";
    // std::cout << "\n--- Final Logs After Re-election ---\n";
    for (int i = 0; i < 3; ++i) {
        if (cluster[i]) {
            cluster[i]->printLog();
        } else {
            std::cout << "Node " << i << " is offline.\n";
        }
    }
    std::cout << "\n--- Simulating Node 0 Recovery from Snapshot ---\n";
    RaftNode recoveredNode0(0);
    
    auto startRestore = std::chrono::high_resolution_clock::now();
    std::vector<Pair> restoredSnapshot = snapshotManager.loadSnapshot("Node_1.json");
    auto endRestore = std::chrono::high_resolution_clock::now();
    auto durationRestore = std::chrono::duration_cast<std::chrono::microseconds>(endRestore - startRestore);
    std::cout << "Snapshot restoration took " << durationRestore.count() << " µs\n";
    std::vector<std::string> restoredLog;
    for (const auto& kv : restoredSnapshot) {
        std::cout << "Restoring entry: " << kv.key << " => " << kv.value << std::endl;
        restoredLog.push_back(kv.value);
    }
    recoveredNode0.overwriteLog(restoredLog);
    recoveredNode0.printLog();
    delete mapper;
    return 0;
}

#include "raft/RaftNode.h"
#include <memory>
#include <iostream>

int main() {
    auto leader = std::make_shared<RaftNode>(0);
    auto follower = std::make_shared<RaftNode>(1);

    leader->startElection();
    leader->becomeLeader();



    leader->appendLog("Test entry 1");
    leader->appendLog("Test entry 2");

    leader->replicateLogToFollower(follower);

    std::cout << "\nLeader log:\n";
    leader->printLog();

    std::cout << "\nFollower log:\n";
    follower->printLog();

    return 0;
}

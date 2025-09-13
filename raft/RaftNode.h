
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

enum class NodeRole {
    Follower,
    Candidate,
    Leader
};

class RaftNode {
public:
    RaftNode(int id);
    int getNodeId() const;
    void receiveHeartbeat(int term, int leaderId);
    void startElection();
    void voteRequest(int candidateId, int term);
    void appendLog(const std::string& entry);
    void overwriteLog(const std::vector<std::string>& newLog);
    void replicateLogToFollower(std::shared_ptr<RaftNode> follower);
    void printLog() const;
    void becomeLeader();
    NodeRole getRole() const;
    int getTerm() const;
    const std::vector<std::string>& getLog() const;

private:
    int nodeId_;
    int currentTerm_;
    int votedFor_;
    NodeRole role_;
    std::vector<std::string> log_;
    std::map<int, bool> votesReceived_;
    int commitIndex_;
    void becomeFollower(int term);
};

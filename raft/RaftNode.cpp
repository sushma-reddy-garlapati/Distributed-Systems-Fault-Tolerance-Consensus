#include "RaftNode.h"
#include <iostream>

RaftNode::RaftNode(int id)
    : nodeId_(id), currentTerm_(0), votedFor_(-1), role_(NodeRole::Follower), commitIndex_(0) {}

int RaftNode::getNodeId() const {
    return nodeId_;
}
void RaftNode::receiveHeartbeat(int term, int leaderId) {
    if (term >= currentTerm_) {
        becomeFollower(term);
        votedFor_ = leaderId;
        std::cout << "Node " << nodeId_ << " received heartbeat from Leader " << leaderId << std::endl;
    }
}

void RaftNode::startElection() {
    currentTerm_++;
    role_ = NodeRole::Candidate;
    votedFor_ = nodeId_;
    votesReceived_.clear();
    votesReceived_[nodeId_] = true;
    std::cout << "Node " << nodeId_ << " started election for term " << currentTerm_ << std::endl;
}

void RaftNode::voteRequest(int candidateId, int term) {
    if (term > currentTerm_ && votedFor_ == -1) {
        votedFor_ = candidateId;
        currentTerm_ = term;
        std::cout << "Node " << nodeId_ << " voted for " << candidateId << " in term " << term << std::endl;
    }
}

void RaftNode::appendLog(const std::string& entry) {
    log_.push_back(entry);
    std::cout << "Node " << nodeId_ << " appended log: " << entry << std::endl;
}


void RaftNode::replicateLogToFollower(std::shared_ptr<RaftNode> follower) {
    if (role_ != NodeRole::Leader) {
        std::cout << "Node " << nodeId_ << " is not a leader. Skipping replication.\n";
        return;
    }
    follower->overwriteLog(log_);
    std::cout << "Node " << nodeId_ << " replicated log to Node " << follower->getNodeId() << "\n";
}
void RaftNode::overwriteLog(const std::vector<std::string>& newLog) {
    log_ = newLog;
}

void RaftNode::printLog() const {
    std::cout << "Node " << nodeId_ << " Log (size: " << log_.size() << "): ";
    for (const auto& entry : log_) {
        std::cout << "[" << entry << "] ";
    }
    std::cout << std::endl;
}

NodeRole RaftNode::getRole() const {
    return role_;
}

int RaftNode::getTerm() const {
    return currentTerm_;
}

const std::vector<std::string>& RaftNode::getLog() const {
    return log_;
}

void RaftNode::becomeLeader() {
    role_ = NodeRole::Leader;
    std::cout << "Node " << nodeId_ << " became Leader for term " << currentTerm_ << std::endl;
}

void RaftNode::becomeFollower(int term) {
    role_ = NodeRole::Follower;
    currentTerm_ = term;
    votedFor_ = -1;
}

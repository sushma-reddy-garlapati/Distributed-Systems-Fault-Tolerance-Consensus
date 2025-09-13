# Fault-Tolerant MapReduce Engine with Raft Consensus

## 🚀 Overview
This project implements a modular distributed system that integrates the Raft consensus protocol with a MapReduce pipeline. It simulates leader election, log replication, crash recovery, and snapshot-based restoration with microsecond-level benchmarking. Designed for fault tolerance, performance profiling. 

## 🧠 Features
- Raft-based leader election and log replication
- Fault-tolerant recovery via snapshot restoration
- MapReduce execution with real computation (WordCount)
- Microsecond-level benchmarking of replication, mapping, reducing, and snapshot I/O
- Modular architecture with clean separation of concerns

## 📊 Benchmark Results
- **Replication latency**: 2–3 µs per follower
- **Mapping time**: 110 µs
- **Reducing time**: 4 µs
- **Snapshot save**: 642 µs
- **Snapshot restore**: 323 µs

## 🛠️ Technologies Used
- C++
- Chrono library (for benchmarking)
- JSON (via `nlohmann/json.hpp`)
- Modular components: `RaftNode`, `Worker`, `Mapper`, `Reducer`, `SnapshotManager`
- Custom test harnesses for unit testing modules (test_raft.cpp, test_worker.cpp, etc.)

## 📦 How to Build and Run
```bash
g++ -std=c++17 main.cpp \
    raft/RaftNode.cpp \
    worker/Worker.cpp \
    snapshots/SnapshotManager.cpp \
    implementations/WordCountMapper.cpp \
    implementations/WordCountReducer.cpp \
    -o mapreduce_main

./mapreduce_main
```

## 🧪 Running Tests
To run unit tests:
```bash
g++ -std=c++17 test_raft.cpp -o test_raft
./test_raft
```
## 📦 Installation
```bash
git clone https://github.com/sushma-reddy-garlapati/Distributed-Systems-Fault-Tolerance-Consensus.git
cd Distributed-Systems-Fault-Tolerance-Consensus
g++ -std=c++17 main.cpp ... -o mapreduce_main
./mapreduce_main
```

## 📁 Project Structure
```text
.
├── raft/                # Raft consensus logic
├── worker/              # MapReduce worker logic
├── implementations/     # WordCount Mapper and Reducer
├── snapshots/           # Snapshot manager and saved state
├── interfaces/          # Abstract Mapper and Reducer interfaces
├── input/               # Sample input data
├── output/              # Result output
├── test_*.cpp           # Unit tests for each module
├── main.cpp             # Entry point
├── README.md            # Project documentation
```

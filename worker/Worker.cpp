#include "Worker.h"

Worker::Worker(Mapper* mapper) : mapper_(mapper) {}

std::vector<Pair> Worker::execute(const std::string& key, const std::string& value) {
    return mapper_->map(key, value);
}

#ifndef PROCESS_H
#define PROCESS_H

#include <string>

namespace processlyzer {

const int kNumberOfProcessFields = 4;

// Struct for holding machine process info, may be expanded later
typedef struct Process {
    int process_id;
    int parent_process_id;
    int user_id;
    std::string process_name;
} Process;

/**
 * Used for comparing processes. Compares the process names and their user ids
 * to see if they are equal.
 * @param process_1 
 * @param process_2 
 * @return true if both the process names or user ids are equal
 */
bool operator==(const Process& process_1, const Process& process_2);

// Hash function which allows using Process as a key in an unordered_map
struct ProcessHash {
  size_t operator()(const Process &process) const {
    size_t part_1 = std::hash<std::string>()(process.process_name);
    size_t part_2 = std::hash<int>()(process.user_id);
 
    return part_1 ^ part_2;
  }
};

} // namespace processlyzer

#endif
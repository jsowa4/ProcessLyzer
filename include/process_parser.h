#ifndef PROCESS_PARSER_H
#define PROCESS_PARSER_H

#include <vector>
#include <unordered_map>
#include <experimental/filesystem>
#include <fstream>

#include "process.h"

namespace processlyzer {

class ProcessParser {
  public:
    ProcessParser() =default;
    
    /**
     * Goes through every directory in the process directory determined by
     * kProcessesDirectory and stores certain process information in a map that
     * can then be accessed via GetProcesses.
     */
    void ReadCurrentProcesses();
    
    /**
     * Returns all the processes with status files found in the process directory.
     * @return a map of user ids to vectors of Process structs
     */
    std::unordered_map<int, std::vector<Process>> GetProcesses();
    
  private:
    // Process directory path for most Unix machines
    std::string kProcessesDirectory = "/proc";
    
    // Map of user ids to a vector of that user's processes
    std::unordered_map<int, std::vector<Process>> processes_;
};

} // namespace processlyzer

#endif
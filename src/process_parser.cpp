#include "../include/process_parser.h"

#include <cstring>
#include <iostream>

namespace processlyzer {

void ProcessParser::ReadCurrentProcesses() {
  for (const auto& directory : std::experimental::filesystem::directory_iterator(kProcessesDirectory)) {
    // Check to see if current directory is a folder for an actual process
    std::string directory_name = directory.path().filename().string();
    bool is_process_dir = true;
    for (char ch : directory_name) {
      if (!isdigit(ch)) {
        // If not a directory of an actual process; processes have integer names
        is_process_dir = false;
        break;
      }
    }
    if (!is_process_dir) continue;
    
    // Open process' status file
    Process this_process;
    //memset(&this_process, 0, sizeof(this_process));
    this_process.process_id = std::stoi(directory_name);
    std::fstream status_input(directory.path().string() + "/status", std::fstream::in);
    if (!status_input) {
      // If could not find a status file just move on to next process
      continue;
    }
    
    // Parse status file for process struct values
    size_t filled_values = 1;
    while (status_input && filled_values < kNumberOfProcessFields) {
      std::string status_line;
      std::string value;
      status_input >> status_line;
      
      if (status_line.length() == 5 && status_line.substr(0, 5) == "Name:") {
        status_input >> value;
        this_process.process_name = value;
        filled_values++;
      }
      
      if (status_line.length() == 5 && status_line.substr(0, 5) == "PPid:") {
        status_input >> value;
        this_process.parent_process_id = std::stoi(value);
        filled_values++;
      }
      
      if (status_line.length() == 4 && status_line.substr(0, 4) == "Uid:") {
        status_input >> value;
        this_process.user_id = std::stoi(value);
        filled_values++;
      }
      
      // Move to next line
      getline(status_input, status_line);
    }
    
    // Add this process to the processes map
    if (processes_.find(this_process.user_id) == processes_.end()) {
      // If this user id is not on the map yet
      processes_[this_process.user_id] = std::vector<Process>();
    }
    processes_[this_process.user_id].push_back(this_process);
  }
}

std::unordered_map<int, std::vector<Process>> ProcessParser::GetProcesses() {
  return processes_;
}

} // namespace processlyzer
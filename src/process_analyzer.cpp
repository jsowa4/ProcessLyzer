#include "process_analyzer.h"

namespace processlyzer {

ProcessAnalyzer::ProcessAnalyzer(std::unordered_map<int, std::vector<Process>> *new_processes) {
  std::fstream input_data(kDefaultSaveFile, std::fstream::in | std::fstream::out | std::fstream::trunc);
  if (!input_data) {
    std::string err_message_as_string = "Could not open " + kDefaultSaveFile + ".\n";
    const char *err_message_as_cstring = err_message_as_string.c_str();
    perror(err_message_as_cstring);
    exit(1);
  }
  
  LoadProcessesFromFile(&input_data);
  input_data.close();
  LoadProcessesFromMap(new_processes);
}

ProcessAnalyzer &ProcessAnalyzer::operator=(const ProcessAnalyzer &other) {
  processes_ = other.processes_;
  abnormal_processes_ = other.abnormal_processes_;
  return *this;
}

ProcessAnalyzer::~ProcessAnalyzer() {
  std::fstream output_data(kDefaultSaveFile, std::fstream::out | std::fstream::trunc);
  if (!output_data) {
    std::string err_message_as_string = "Could not open " + kDefaultSaveFile + ".\n";
    const char *err_message_as_cstring = err_message_as_string.c_str();
    perror(err_message_as_cstring);
    exit(1);
  }
  
  SaveProcessesToFile(&output_data);
  output_data.close();
}

std::vector<Process> ProcessAnalyzer::GetAbnormalProcesses() {
  return abnormal_processes_;
}

std::unordered_map<Process, unsigned long, ProcessHash> ProcessAnalyzer::GetProcessesMap() {
  return processes_;
}

void ProcessAnalyzer::LoadProcessesFromMap(std::unordered_map<int, std::vector<Process>> *new_processes) {
  abnormal_processes_.clear();
  for (const auto& uid_process_pair : *new_processes) {
    for (const auto& process : uid_process_pair.second) {
      if (processes_.find(process) == processes_.end()) {
        // If process has not been seen before
        if (IsIgnoredProcess(process)) {
          // If process is meant to be ignored (such as kworker processes)
          continue;
        }

        std::pair<Process, unsigned long> entry(process, 1);
        processes_.insert(entry);
        abnormal_processes_.push_back(process);
      } else {
        // If process has been seen before increment occurrences
        if (processes_[process] + 1 > 0) {
          // Prevents overflow 
          processes_[process] += 1;
        }
      }
    }
  }
}

bool ProcessAnalyzer::IsIgnoredProcess(const Process& process) {
  for (const std::string& ignored_process : kIgnoredProcesses) {
    // Check if process is an ignorable process
    if (ignored_process == process.process_name.substr(0, ignored_process.length())) {
      return true;
    }
  }
  
  return false;
}

void ProcessAnalyzer::SaveProcessesToFile(std::fstream *output_data) {
  for (const auto& process_n_occurrences : processes_) {
    const Process& process = process_n_occurrences.first;
    unsigned long occurrences = process_n_occurrences.second;
    *output_data << process.process_name << " ";
    *output_data << process.user_id << " ";
    *output_data << process.process_id << " ";
    *output_data << process.parent_process_id << " ";
    *output_data << occurrences << "\n";
  }
}

void ProcessAnalyzer::LoadProcessesFromFile(std::fstream *input_data) {
  std::string line;
  while (std::getline(*input_data, line)) {
    std::vector<std::string> tokens;
    if (SplitString(tokens, line) == 5) {
      Process process = {
          stoi(tokens[2]),    // process_id
          stoi(tokens[3]),    // parent_process_id
          stoi(tokens[1]),    // user_id
          tokens[0]           // process_name
      };
      unsigned long occurrences = stoi(tokens[4]);
      std::pair<Process, unsigned long> entry(process, occurrences);
      processes_.insert(entry);
    }
  }
}

size_t ProcessAnalyzer::SplitString(std::vector<std::string> &tokens, std::string str) {
  size_t num_tokens = 0;
  char *token = strtok(const_cast<char*>(str.c_str()), " ");
  while (token != NULL) {
    size_t token_length = strlen(token);
    if (token[token_length - 1] == '\n') {
      // Remove new lines from end of tokens
      if (token_length == 1) {
        token = strtok(NULL, " ");
        continue;
      }
      token[token_length - 1] = 0;
    }
    
    tokens.emplace_back(token);
    token = strtok(nullptr, " ");
    num_tokens++;
  }
  
  return num_tokens;
}

}

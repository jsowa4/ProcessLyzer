#ifndef PROCESS_ANALYZER_H
#define PROCESS_ANALYZER_H

#include <unordered_map>
#include <vector>
#include <fstream>
#include <cstring>

#include "process.h"
#include "ignored_processes.h"

namespace processlyzer {

class ProcessAnalyzer {
 public:
  /**
   * Default Constructor
   */
   ProcessAnalyzer() =default;
  
  /**
   * Initializes the process analyzer first with the saved process history found in
   * the default save file, and then with the updated process map from the process parser.
   * @param new_processes 
   */
  explicit ProcessAnalyzer(std::unordered_map<int, std::vector<Process>> *new_processes);
  
  /**
   * Copy assignment operator.
   */
  ProcessAnalyzer &operator=(const ProcessAnalyzer& other);
  
  /**
   * Saves the process history to the default save file upon exit.
   */
  ~ProcessAnalyzer();
  
  /**
   * Updates the currently loaded process history with the current process map from
   * the process parser.
   * @param new_processes 
   */
  void LoadProcessesFromMap(std::unordered_map<int, std::vector<Process>> *new_processes);
  
  /**
   * Gets a map of all processes to their occurrences.
   * @return unordered map of all processes to their occurrences
   */
  std::unordered_map<Process, unsigned long, ProcessHash> GetProcessesMap();
  
  /**
   * Gets a vector of all processes which were not found in the process history.
   * @return vector of new processes
   */
  std::vector<Process> GetAbnormalProcesses();
  
 private:
  // Default save directory to be created if it does not exist
  static constexpr char kDefaultSaveDirectory[] = "../data";
  
  // Path to save file for known processes
  const std::string kDefaultSaveFile = "data/proc_history.txt";
  
  // A map tracking all known Processes mapped to their occurrences
  std::unordered_map<Process, unsigned long, ProcessHash> processes_;
  
  // A vector of yet unseen processes
  std::vector<Process> abnormal_processes_;
  
  /**
   * Saves the process history to the file at the specified output file stream.
   * @param output_data the write fstream object
   */
  void SaveProcessesToFile(std::fstream *output_data);
  
  /**
   * Reads the saved process history from the file specified by the file stream.
   * Truncates the file so that it can be updated later.
   * @param input_data the read file stream object
   */
  void LoadProcessesFromFile(std::fstream *input_data);
  
  /**
   * Splits a line of formatted input separated by spaces into tokens. Removes any new line
   * characters which would be found at the end of a line.
   * @param tokens the vector that will be filled with the input's tokens
   * @param str the input string
   * @return the number of tokens that were found in the string
   */
  size_t SplitString(std::vector<std::string> &tokens, std::string str);
  
  /**
   * Checks if the specified process appears in the ignored processes array.
   * @param process the process to check
   * @return true if the process appears in the array of processes to ignore,
   *    false otherwise
   */
  static bool IsIgnoredProcess(const Process& process);
};

}

#endif  // PROCESS_ANALYZER_H

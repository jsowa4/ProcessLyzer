#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "../include/process_parser.h"
#include "../include/process_analyzer.h"

volatile sig_atomic_t interrupt_flag = false;
const std::string version = "v1.2";

// Used so that when run, a sudden interrupt does not corrupt saved data
void InterruptHandler(int signal_num);

int main () {
  // Welcome Message and Setup
  signal(SIGINT, InterruptHandler);     // ctrl+c
  signal(SIGTERM, InterruptHandler);    // clion stop button sends this signal
  processlyzer::ProcessParser parser;
  std::cout << "Welcome to Processlyzer " << version << "!" << std::endl;
  std::cout << "Starting detection of processes..." << std::endl;
  
  // First process check
  parser.ReadCurrentProcesses();
  std::unordered_map<int, std::vector<processlyzer::Process>> read_processes = parser.GetProcesses();
  processlyzer::ProcessAnalyzer analyzer(&read_processes);
  
  while (!interrupt_flag) {
    // Runs until clion stop button is hit or an interrupt is sent
    sleep(10);
    parser.ReadCurrentProcesses();
    read_processes = parser.GetProcesses();
    analyzer.LoadProcessesFromMap(&read_processes);
  
    std::cout << std::endl << "Newly detected processes:" << std::endl;
    size_t num_abnormal_processes = 0;
    for (const processlyzer::Process& process : analyzer.GetAbnormalProcesses()) {
      std::cout << process.process_name << std::endl;
      num_abnormal_processes++;
    }
    
    if (num_abnormal_processes == 0) {
      std::cout << "No new processes detected!" << std::endl;
    }
  }
  
  return 0;
}

void InterruptHandler(int signal_num) {
  interrupt_flag = true;
} 
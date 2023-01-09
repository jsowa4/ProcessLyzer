#ifndef PROCESSANALYZER_IGNORED_PROCESSES_H
#define PROCESSANALYZER_IGNORED_PROCESSES_H

#include <vector>
#include <string>

namespace processlyzer {

// If there is a process for this program to ignore, add it to this vector
// initialization list
const std::vector<std::string> kIgnoredProcesses {
  "kworker/",
  "irq/"
};

} // namespace processlyzer

#endif  // PROCESSANALYZER_IGNORED_PROCESSES_H

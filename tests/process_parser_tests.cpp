#include <catch2/catch.hpp>
#include <iostream>

#include "../include/process_parser.h"

namespace processlyzer {

TEST_CASE("Read current processes test") {
  processlyzer::ProcessParser parser;
  parser.ReadCurrentProcesses();
  std::unordered_map<int, std::vector<Process>> processes = parser.GetProcesses();
  
  SECTION("Finds processes for current user") {
    REQUIRE(processes.find(1000) != processes.end());
  }
  
  SECTION("Finds processes for root") {
    REQUIRE(processes.find(0) != processes.end());
  }
  
  SECTION("Finds systemd process") {
    Process process = processes.at(0).front();
    REQUIRE(process.process_name == "systemd");
  }

  SECTION("Finds Clion script process") {
    Process process;
    bool found = false;
    for (auto user_process : processes.at(1000)) {
      if (user_process.process_name == "clion.sh") {
        found = true;
      }
    }

    REQUIRE(found == true);
  }
}

}
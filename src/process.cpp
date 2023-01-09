#include "process.h"

namespace processlyzer {

bool operator==(const Process& process_1, const Process& process_2) {
  return process_1.process_name == process_2.process_name
         && process_1.user_id == process_2.user_id;
}

} // namespace processlyzer
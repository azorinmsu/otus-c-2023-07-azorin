#include "logger.h"

int main() {
  init_consoleLogger(stdout);
  init_fileLogger("example.txt");

  LOG_TRACE("TRACE");
  LOG_DEBUG("DEBUG");
  LOG_INFO("INFO");
  LOG_ERROR("ERROR");
  LOG_FATAL("FATAL");
}

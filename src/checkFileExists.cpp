#include "functions.h"

/// Returns true if file does exist
bool checkFileExists(const std::string &file)
{
  std::ifstream infile(file);

  return static_cast<bool>(infile);
}

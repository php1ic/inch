#include "functions.h"

/// Returns true if file does exist
bool checkFileExists(const std::string &file)
{
  struct stat table;

  return !stat(file.c_str(), &table);
}

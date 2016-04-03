#include "functions.h"

bool checkFileExists(const std::string &file)
{
  struct stat table;

  return !stat(file.c_str(), &table);
}

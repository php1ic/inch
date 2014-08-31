#include "include/functions.h"

int countLinesInFile(std::ifstream &inFile)
{
  //Get line count
  int line_count=std::count(std::istreambuf_iterator<char>(inFile),
			    std::istreambuf_iterator<char>(), '\n');

  //Counting lines leaves stream pointing to end, put it back to start.
  inFile.seekg(0,inFile.beg);

  return line_count;
}

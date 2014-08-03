#include "include/functions.h"

template <typename Type>
void extractValue(const std::string &line,
		  const int &start,
		  const int &end,
		  Type &value
		  )
{
  std::stringstream in(line.substr(start,end));
  in >> value;
}

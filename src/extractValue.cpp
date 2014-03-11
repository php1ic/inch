#include "include/functions.h"

template <typename Type>
void extractValue(const std::string *line,
		  const int &start,
		  const int &end,
		  Type &value
		  )
{
  std::stringstream in;
  std::string temp("");

  temp.assign(line->begin()+start,line->begin()+end);
  in << temp;
  in >> value;

  in.str("");
  in.clear();
}

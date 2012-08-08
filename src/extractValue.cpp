#include "functions.h"

template <typename Type>
void extractValue(std::string *line, int start, int end, Type &value)
{
  std::stringstream in;
  std::string *temp = new std::string;

  temp->assign(line->begin()+start,line->begin()+end);
  in << *temp;
  in >> value;

  in.str("");
  in.clear();

  delete temp;
}

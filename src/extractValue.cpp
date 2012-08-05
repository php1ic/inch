#include "functions.h"

template <typename Type> 
void extractValue(std::string *line, int start, int end, Type &value)
{
  std::stringstream test;
  std::string *temp = new std::string;
  
  temp->assign(line->begin()+start,line->begin()+end);
  test << *temp;
  test >> value;

  test.str("");
  test.clear();
  
  delete temp;
}

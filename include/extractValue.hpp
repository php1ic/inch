#ifndef EXTRACTVALUE_HPP
#define EXTRACTVALUE_HPP

#include <string>
#include <sstream>

template <typename Type>
inline void extractValue(const std::string &line,
                         const int start,
                         const int end,
                         Type &value
                         )
{
  std::stringstream in(line.substr(start,end-start));
  in >> value;
}

#endif // EXTRACTVALUE_HPP

#ifndef PARTITION_H
#define PARTITION_H

#include <string>
#include <vector>

class partition
{
 public:
  partition();
  ~partition();
  std::vector<std::string> colour;
  std::vector<float> value;
  std::vector<bool> draw;
};

#endif

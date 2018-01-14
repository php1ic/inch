#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <string>
#include <vector>

class partition
{
 public:
  partition();
  ~partition() = default;

  std::vector<std::string> colour;
  std::vector<double> value;
  std::vector<bool> draw;
};

#endif // PARTITION_HPP

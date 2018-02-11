#ifndef PARTITION_HPP
#define PARTITION_HPP

#include <string>
#include <vector>

class partition
{
 public:
  //Constructors
  //default
  partition();
  //copy
  partition(const partition&) = default;
  //move
  partition(partition&&) = default;

  //Assignment
  //copy
  partition& operator=(const partition&) = default;
  //move
  partition& operator=(partition&&) = default;

  //Destructors
  ~partition() = default;

  std::vector<std::string> colour;
  std::vector<double> value;
  std::vector<bool> draw;
};

#endif // PARTITION_HPP

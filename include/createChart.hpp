#ifndef CREATECHART_HPP
#define CREATECHART_HPP

#include "functions.hpp"

class createChart
{
public:
  //Constructor
  //default
  createChart(std::unique_ptr<inputs> &draw,
              std::unique_ptr<partition> &part,
              std::unique_ptr<MassTable> &table);

  //copy
  createChart(const createChart&) = default;
  //move
  createChart(createChart&&) = default;

  //Assignment
  //copy
  createChart& operator=(const createChart&) = default;
  //move
  createChart& operator=(createChart&&) = default;

  //Destructor
  ~createChart() = default;
};

#endif // CREATECHART_HPP

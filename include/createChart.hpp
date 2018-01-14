#ifndef CREATECHART_HPP
#define CREATECHART_HPP

#include "functions.hpp"

class createChart
{
public:
  createChart(std::unique_ptr<inputs> &draw,
              std::unique_ptr<partition> &part,
              std::vector<Nuclide> &nuc);

  ~createChart() = default;
};

#endif // CREATECHART_HPP

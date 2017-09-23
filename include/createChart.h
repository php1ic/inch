#ifndef CREATECHART_H
#define CREATECHART_H

#include "functions.h"

class createChart
{
public:
  createChart(std::unique_ptr<inputs> &draw,
              std::unique_ptr<partition> &part,
              std::vector<Nuclide> &nuc);

  ~createChart();
};

#endif // CREATECHART_H

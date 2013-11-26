#ifndef CREATECHART_H
#define CREATECHART_H

#include "include/functions.h"

class createChart
{
public:
  createChart(inputs *draw,
	      partition *part,
	      std::vector<Nuclide> &nuc,
	      bool &inputfile,
	      std::vector<std::string> &arguments);

  createChart(inputs *draw,
	      partition *part,
	      std::vector<Nuclide> &nuc);

  ~createChart();
};

#endif // CREATECHART_H

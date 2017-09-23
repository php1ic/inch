#include "createChart.h"

int main(int argc, char *argv[])
{
  std::unique_ptr<inputs> draw(new inputs);
  draw->showBanner();

  std::vector<std::string> arguments(argv,argv+argc);
  draw->valid_console = draw->readConsoleArguments(arguments);

  if ( draw->valid_console == 2 )
    {
      return 0;
    }

  partition *part = new partition;
  std::vector<Nuclide> nuc;

  createChart *chart = new createChart(draw,part,nuc);
  delete chart;

  draw->writeOptionFile();

  delete part;

  std::cout << "Enjoy\n" << std::endl;

  return 0;
}

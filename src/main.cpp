#include "createChart.h"

int main(int argc, char *argv[])
{
  std::unique_ptr<inputs> draw = std::make_unique<inputs>();
  draw->showBanner();

  draw->valid_console = draw->readConsoleArguments(std::vector<std::string> (argv,argv+argc));

  if ( draw->valid_console == 2 )
    {
      return 0;
    }

  std::unique_ptr<partition> part = std::make_unique<partition>();
  std::vector<Nuclide> nuc;

  createChart(draw,part,nuc);

  draw->writeOptionFile();

  std::cout << "Enjoy\n" << std::endl;

  return 0;
}

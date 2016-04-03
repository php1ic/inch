#include "createChart.h"

int main(int argc, char *argv[])
{
  inputs *draw = new inputs;

  if ( argc == 2
      && (argv[1] == std::string("--version") || argv[1] == std::string("-v"))
      )
    {
      draw->showVersion();
      delete draw;
      return 0;
    }

  partition *part = new partition;
  std::vector<Nuclide> nuc;

  draw->showBanner();
  draw->constructFullyQualifiedPaths();

  if ( argc > 1 )
    {
      std::vector<std::string> arguments(argv,argv+argc);
      createChart *chart = new createChart(draw,part,nuc,arguments);
      delete chart;
    }
  else
    {
      createChart *chart = new createChart(draw,part,nuc);
      delete chart;
    }

  draw->writeOptionFile();

  std::cout << "Enjoy\n"
            << "\nTo run again with the same options: " << argv[0]
            << " -i options.in\n" << std::endl;

  delete draw;
  delete part;

  std::cout << std::endl;

  return 0;
}

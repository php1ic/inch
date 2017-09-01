#include "createChart.h"

int main(int argc, char *argv[])
{
  inputs *draw = new inputs;

  if ( argc == 2 )
    {
      if ( argv[1] == std::string("--version") || argv[1] == std::string("-v") )
        {
          draw->showVersion();
        }
      else if ( argv[1] == std::string("--help") || argv[1] == std::string("-h") )
        {
          draw->showUsage(std::string(argv[0]));
        }
      else
        {
          std::cerr << "\n***ERROR***: Unkown single option " << argv[1] << " exiting...\n";
          draw->showUsage(std::string(argv[0]));
        }

      delete draw;
      return 0;
    }

  partition *part = new partition;
  std::vector<Nuclide> nuc;

  draw->showBanner();

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
            << " -i " << draw->options << "\n"
            << std::endl;

  delete draw;
  delete part;

  std::cout << std::endl;

  return 0;
}

#include "functions.hpp"

void drawEPSMagicNumbers(const std::unique_ptr<inputs> &draw,
                         std::ofstream &outFile
                         )
{
  std::cout << "Drawing magic numbers";

  struct MagicNumber
  {
    MagicNumber(int N, double minN, double maxN, double minZ, double maxZ):
      number{N},
      min_n{minN},
      max_n{maxN},
      min_z{minZ},
      max_z{maxZ}
    {}

    int number;
    double min_n;
    double max_n;
    double min_z;
    double max_z;
  };

  std::vector<MagicNumber> values = {
    {  2,  0.0,  21.5,  0.0, 11.5},
    {  8,  1.0,  29.5,  0.0, 21.5},
    { 20,  7.5,  51.5,  1.5, 32.5},
    { 28, 16.5,  55.5,  7.5, 37.5},
    { 50, 45.5,  91.5, 19.5, 55.5},
    { 82, 81.5, 139.5, 43.5, 83.5},
    {126,  0.0,   0.0, 76.5, 94.5} // Z<=118 so Z=126 magic number is not needed
  };

  double max  =  0.0;
  double min  =  0.0;
  double low  = -0.25;
  double high =  1.25;

  outFile << "\n%-----------------\n"
          << "%- Magic Numbers -\n"
          << "%-----------------\n"
          << "gs\n"
          << "black rgb\n"
          << "1 u div sl" << std::endl;

  for ( const auto magic: values)
    {
      //-Proton
      if (   draw->Zmax >= magic.number
             && draw->Zmin <= magic.number
             )
        {
          if ( (min = magic.min_n - draw->Nmin) < 0.0 )
            {
              min = low;
            }

          if ( (max = magic.max_n - draw->Nmin) > (draw->Nmax - draw->Nmin + high) )
            {
              max = draw->Nmax - draw->Nmin + high;
            }

          outFile << "%Z=" << magic.number << " magic number\n"
                  << min << " " << magic.number - draw->Zmin+1 << " m " << max - min << " 0 rl\n"
                  << min << " " << magic.number - draw->Zmin   << " m " << max - min << " 0 rl st" << std::endl;
        }

      //-Neutron
      if (   draw->Nmax >= magic.number
             && draw->Nmin <= magic.number
             )
        {
          if ( (min = magic.min_z - draw->Zmin) < 0.0 )
            {
              min = low;
            }

          if ( (max = magic.max_z - draw->Zmin) > (draw->Zmax - draw->Zmin + high) )
            {
              max = draw->Zmax - draw->Zmin + high;
            }

          outFile << "%N=" << magic.number << " magic number\n"
                  << magic.number - draw->Nmin+1 << " " << min << " m 0 " << max - min << " rl\n"
                  << magic.number - draw->Nmin   << " " << min << " m 0 " << max - min << " rl st" << std::endl;
        }
    }

  outFile << "gr" << std::endl;

  std::cout << " - done" << std::endl;
}

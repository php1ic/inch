#include "functions.hpp"

void setIsotopeAttributes(std::vector<Nuclide> &in,
                          std::unique_ptr<Partition> &part,
                          const std::unique_ptr<inputs> &draw
                          )
{
  // Using the region specified, flag that the isotope should be drawn
  // together with the corresponding part of the key.
  for ( auto it=in.begin(); it!=in.end(); ++it )
    {
      if (   it->Z    >= draw->Zmin
          && it->Z    <= draw->Zmax
          && it->N    >= draw->Nmin
          && it->N    <= draw->Nmax
          && it->exp  != draw->experimental
          && it->rich  % draw->np_rich == 0
          )
        {
          /// Error on mass excess units of keV
          if ( draw->choice == "a" )
            {
              it->show = 1;

              const double me = draw->AME ? it->AME_dME : it->NUBASE_dME;

              /// There should be 2 partitions to differentiate stable isotopes,
              /// if the isotope is stable, we can avoid a lot of checking
              if ( it->decay == "stable" )
                {
                  const int index = (me <= part->values.front().value) ? 0 : 1;

                  it->colour = part->values[index].colour;
                  part->values[index].draw = true;
                  continue;
                }

              /// Only get here if the isotope is not stable
              /// Can skip the first 2 partitions as they are only for stable isotopes
              for ( auto val = std::next(std::begin(part->values), 2); val != std::end(part->values); ++val )
                {
                  if ( me <= val->value )
                    {
                      it->colour = val->colour;
                      val->draw = true;
                      break;
                    }
                }
            }
          /// Relative error on mass excess units of keV
          else if ( draw->choice == "b" )
            {
              it->show = 1;

              constexpr double min = 1.0e-7;
              const double dme =
                [&]()
                {
                  /// Be explicit as switch statements implicitly convert bool -> int
                  switch ( static_cast<int>(draw->AME) )
                    {
                    case 1: /// true
                      return (fabs(it->AME_ME) < min)
                        ? 0.0
                        : fabs(it->AME_dME/it->AME_ME);
                    case 0: /// false
                    default:
                      return (fabs(it->NUBASE_ME) < min)
                        ? 0.0
                        : fabs(it->NUBASE_dME/it->NUBASE_ME);
                    }
                }();

              for ( auto& val: part->values )
                {
                  if ( dme <= val.value )
                    {
                      it->colour = val.colour;
                      val.draw = true;
                      break;
                    }
                }
            }
          /// Major ground-state decay mode
          else if ( draw->choice == "c" )
            {
              it->show = 1;

              if ( it->decay == "stable" )
                {
                  it->colour = part->values[0].colour;
                  part->values[0].draw = true;
                }
              else if ( it->decay == "A" )
                {
                  it->colour = part->values[1].colour;
                  part->values[1].draw = true;
                }
              else if ( it->decay == "B-" )
                {
                  it->colour = part->values[2].colour;
                  part->values[2].draw = true;
                }
              else if ( it->decay == "B+" )
                {
                  it->colour = part->values[3].colour;
                  part->values[3].draw = true;
                }
              else if ( it->decay == "SF" )
                {
                  it->colour = part->values[4].colour;
                  part->values[4].draw = true;
                }
              else if ( it->decay == "n" )
                {
                  it->colour = part->values[5].colour;
                  part->values[5].draw = true;
                }
              else if ( it->decay == "2n" )
                {
                  it->colour = part->values[6].colour;
                  part->values[6].draw = true;
                }
              else if ( it->decay == "p" )
                {
                  it->colour = part->values[7].colour;
                  part->values[7].draw = true;
                }
              else if ( it->decay == "2p" )
                {
                  it->colour = part->values[8].colour;
                  part->values[8].draw = true;
                }
              else if ( it->decay == "unknown" )
                {
                  it->colour = part->values[9].colour;
                  part->values[9].draw = true;
                }
              else if ( it->decay == "EC" )
                {
                  it->colour = part->values[10].colour;
                  part->values[10].draw = true;
                }
            }
          /// Half-life of ground-state
          else if ( draw->choice == "d" )
            {
              it->show = 1;

              for ( auto& val: part->values )
                {
                  if ( it->hl <= val.value )
                    {
                      it->colour = val.colour;
                      val.draw = true;
                      break;
                    }
                }
            }
          /// 1st isomer energy
          else if ( draw->choice == "e" )
            {
              if ( it->energy_levels.size() > 0 && it->energy_levels.front().level == 1 )
                {
                  it->show = 1;

                  for ( auto& val: part->values )
                    {
                      if ( it->energy_levels.front().energy <= val.value )
                        {
                          it->colour = val.colour;
                          val.draw = true;
                          break;
                        }
                    }
                }
              /// As not every nucleus has an isomer, draw empty boxes as a visual aid
              /// This relies on the vector being sorted as it was in the data file
              else
                {
                  it->show = 2;
                  it->colour = part->values.back().colour;
                  part->values.back().draw = true;
                }
            }
        }
    }
}

#include "functions.h"

void setColours(std::unique_ptr<partition> &part,
                const std::unique_ptr<inputs> &draw
                )
{
  //Mass excess
  if ( draw->choice == "a" )
    {
      part->value.emplace_back(3.0);
      part->value.emplace_back(10.0);
      part->value.emplace_back(20.0);
      part->value.emplace_back(50.0);
      part->value.emplace_back(100.0);

      part->colour.emplace_back("black");
      part->colour.emplace_back("magenta");
      part->colour.emplace_back("blue");
      part->colour.emplace_back("cyan");
      part->colour.emplace_back("green");
      part->colour.emplace_back("yellow");
      part->colour.emplace_back("orange");
      part->colour.emplace_back("red");
    }
  //Relative error on mass excess
  else if ( draw->choice == "b" )
    {
      part->value.emplace_back(2.0e-05);
      part->value.emplace_back(1.0e-04);
      part->value.emplace_back(5.0e-04);
      part->value.emplace_back(1.0e-03);
      part->value.emplace_back(5.0e-03);

      part->colour.emplace_back("black");
      part->colour.emplace_back("blue");
      part->colour.emplace_back("green");
      part->colour.emplace_back("yellow");
      part->colour.emplace_back("orange");
      part->colour.emplace_back("red");
    }
  //Decay method
  else if ( draw->choice == "c" )
    {
      part->colour.emplace_back("black");
      part->colour.emplace_back("yellow");
      part->colour.emplace_back("blue");
      part->colour.emplace_back("red");
      part->colour.emplace_back("magenta");
      part->colour.emplace_back("cyan");
      part->colour.emplace_back("darkgreen");
      part->colour.emplace_back("navyblue");
      part->colour.emplace_back("purple");
      part->colour.emplace_back("green");
      part->colour.emplace_back("orange");
    }
  //Decay half-life
  else if ( draw->choice == "d" )
    {
      //-Use units of seconds
      part->value.emplace_back(0.1);              // 0.1 second
      part->value.emplace_back(3.0);              // 3 seconds
      part->value.emplace_back(120.0);            // 2 mins
      part->value.emplace_back(3600.0);           // 1 hour
      part->value.emplace_back(86400.0);          // 1 day
      part->value.emplace_back(31557600.0);       // 1 year
      part->value.emplace_back(31557600.0*1.0e09);// 1e9 years

      part->colour.emplace_back("red");
      part->colour.emplace_back("orange");
      part->colour.emplace_back("yellow");
      part->colour.emplace_back("green");
      part->colour.emplace_back("cyan");
      part->colour.emplace_back("blue");
      part->colour.emplace_back("magenta");
      part->colour.emplace_back("black");
    }
  //Isomer energy
  else if ( draw->choice == "e" )
    {
      part->value.emplace_back(20.0);
      part->value.emplace_back(50.0);
      part->value.emplace_back(100.0);
      part->value.emplace_back(500.0);
      part->value.emplace_back(1000.0);

      part->colour.emplace_back("red");
      part->colour.emplace_back("orange");
      part->colour.emplace_back("yellow");
      part->colour.emplace_back("green");
      part->colour.emplace_back("blue");
      part->colour.emplace_back("magenta");
      // For an empty square, this should always be last.
      part->colour.emplace_back("white");
    }
}

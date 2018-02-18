#include "partition.hpp"

partition::partition():
  draw(12,false)
{
}


void partition::setColours(const std::unique_ptr<inputs> &draw)
{
  //Mass excess
  if ( draw->choice == "a" )
    {
      value.emplace_back(3.0);
      value.emplace_back(10.0);
      value.emplace_back(20.0);
      value.emplace_back(50.0);
      value.emplace_back(100.0);

      colour.emplace_back("black");
      colour.emplace_back("magenta");
      colour.emplace_back("blue");
      colour.emplace_back("cyan");
      colour.emplace_back("green");
      colour.emplace_back("yellow");
      colour.emplace_back("orange");
      colour.emplace_back("red");
    }
  //Relative error on mass excess
  else if ( draw->choice == "b" )
    {
      value.emplace_back(2.0e-05);
      value.emplace_back(1.0e-04);
      value.emplace_back(5.0e-04);
      value.emplace_back(1.0e-03);
      value.emplace_back(5.0e-03);

      colour.emplace_back("black");
      colour.emplace_back("blue");
      colour.emplace_back("green");
      colour.emplace_back("yellow");
      colour.emplace_back("orange");
      colour.emplace_back("red");
    }
  //Decay method
  else if ( draw->choice == "c" )
    {
      colour.emplace_back("black");
      colour.emplace_back("yellow");
      colour.emplace_back("blue");
      colour.emplace_back("red");
      colour.emplace_back("magenta");
      colour.emplace_back("cyan");
      colour.emplace_back("darkgreen");
      colour.emplace_back("navyblue");
      colour.emplace_back("purple");
      colour.emplace_back("green");
      colour.emplace_back("orange");
    }
  //Decay half-life
  else if ( draw->choice == "d" )
    {
      //-Use units of seconds
      value.emplace_back(0.1);              // 0.1 second
      value.emplace_back(3.0);              // 3 seconds
      value.emplace_back(120.0);            // 2 mins
      value.emplace_back(3600.0);           // 1 hour
      value.emplace_back(86400.0);          // 1 day
      value.emplace_back(31557600.0);       // 1 year
      value.emplace_back(31557600.0*1.0e09);// 1e9 years

      colour.emplace_back("red");
      colour.emplace_back("orange");
      colour.emplace_back("yellow");
      colour.emplace_back("green");
      colour.emplace_back("cyan");
      colour.emplace_back("blue");
      colour.emplace_back("magenta");
      colour.emplace_back("black");
    }
  //Isomer energy
  else if ( draw->choice == "e" )
    {
      value.emplace_back(20.0);
      value.emplace_back(50.0);
      value.emplace_back(100.0);
      value.emplace_back(500.0);
      value.emplace_back(1000.0);

      colour.emplace_back("red");
      colour.emplace_back("orange");
      colour.emplace_back("yellow");
      colour.emplace_back("green");
      colour.emplace_back("blue");
      colour.emplace_back("magenta");
      // For an empty square, this should always be last.
      colour.emplace_back("white");
    }
}

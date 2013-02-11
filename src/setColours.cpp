#include "functions.h"

void setColours(std::vector<std::string> &partition_colour, std::vector<float> &partition_value, const inputs *draw)
{
  //Mass excess
  if (draw->choice == "a")
    {
      partition_value.push_back(3.0);
      partition_value.push_back(10.0);
      partition_value.push_back(20.0);
      partition_value.push_back(50.0);
      partition_value.push_back(100.0);

      partition_colour.push_back("black");
      partition_colour.push_back("magenta");
      partition_colour.push_back("blue");
      partition_colour.push_back("cyan");
      partition_colour.push_back("green");
      partition_colour.push_back("yellow");
      partition_colour.push_back("orange");
      partition_colour.push_back("red");
    }
  //Relative error on mass excess
  else if (draw->choice == "b")
    {
      partition_value.push_back(2.0e-05);
      partition_value.push_back(1.0e-04);
      partition_value.push_back(5.0e-04);
      partition_value.push_back(1.0e-03);
      partition_value.push_back(5.0e-03);

      partition_colour.push_back("black");
      partition_colour.push_back("blue");
      partition_colour.push_back("green");
      partition_colour.push_back("yellow");
      partition_colour.push_back("orange");
      partition_colour.push_back("red");
    }
  //Decay method
  else if (draw->choice == "c")
    {
      partition_colour.push_back("black");
      partition_colour.push_back("yellow");
      partition_colour.push_back("blue");
      partition_colour.push_back("red");
      partition_colour.push_back("magenta");
      partition_colour.push_back("cyan");
      partition_colour.push_back("darkgreen");
      partition_colour.push_back("navyblue");
      partition_colour.push_back("purple");
      partition_colour.push_back("green");
      partition_colour.push_back("orange");
    }
  //Decay half-life
  else if (draw->choice == "d")
    {
      //-Use units of seconds
      partition_value.push_back(0.1);              // 0.1 second
      partition_value.push_back(3.0);              // 3 seconds
      partition_value.push_back(120.0);            // 2 mins
      partition_value.push_back(3600.0);           // 1 hour
      partition_value.push_back(86400.0);          // 1 day
      partition_value.push_back(31557600.0);       // 1 year
      partition_value.push_back(31557600.0*1.0e09);// 1e9 years

      partition_colour.push_back("red");
      partition_colour.push_back("orange");
      partition_colour.push_back("yellow");
      partition_colour.push_back("green");
      partition_colour.push_back("cyan");
      partition_colour.push_back("blue");
      partition_colour.push_back("magenta");
      partition_colour.push_back("black");
    }
  //Isomer energy
  else if (draw->choice == "e")
    {
      partition_value.push_back(20.0);
      partition_value.push_back(50.0);
      partition_value.push_back(100.0);
      partition_value.push_back(500.0);
      partition_value.push_back(1000.0);

      partition_colour.push_back("red");
      partition_colour.push_back("orange");
      partition_colour.push_back("yellow");
      partition_colour.push_back("green");
      partition_colour.push_back("blue");
      partition_colour.push_back("magenta");
      partition_colour.push_back("white"); // For an empty square, should always be last.
    }
}

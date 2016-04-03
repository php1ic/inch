#include "functions.h"

void setColours(partition *part,
		const inputs *draw
		)
{
  //Mass excess
  if (draw->choice == "a")
    {
      part->value.push_back(3.0);
      part->value.push_back(10.0);
      part->value.push_back(20.0);
      part->value.push_back(50.0);
      part->value.push_back(100.0);

      part->colour.push_back("black");
      part->colour.push_back("magenta");
      part->colour.push_back("blue");
      part->colour.push_back("cyan");
      part->colour.push_back("green");
      part->colour.push_back("yellow");
      part->colour.push_back("orange");
      part->colour.push_back("red");
    }
  //Relative error on mass excess
  else if (draw->choice == "b")
    {
      part->value.push_back(2.0e-05);
      part->value.push_back(1.0e-04);
      part->value.push_back(5.0e-04);
      part->value.push_back(1.0e-03);
      part->value.push_back(5.0e-03);

      part->colour.push_back("black");
      part->colour.push_back("blue");
      part->colour.push_back("green");
      part->colour.push_back("yellow");
      part->colour.push_back("orange");
      part->colour.push_back("red");
    }
  //Decay method
  else if (draw->choice == "c")
    {
      part->colour.push_back("black");
      part->colour.push_back("yellow");
      part->colour.push_back("blue");
      part->colour.push_back("red");
      part->colour.push_back("magenta");
      part->colour.push_back("cyan");
      part->colour.push_back("darkgreen");
      part->colour.push_back("navyblue");
      part->colour.push_back("purple");
      part->colour.push_back("green");
      part->colour.push_back("orange");
    }
  //Decay half-life
  else if (draw->choice == "d")
    {
      //-Use units of seconds
      part->value.push_back(0.1);              // 0.1 second
      part->value.push_back(3.0);              // 3 seconds
      part->value.push_back(120.0);            // 2 mins
      part->value.push_back(3600.0);           // 1 hour
      part->value.push_back(86400.0);          // 1 day
      part->value.push_back(31557600.0);       // 1 year
      part->value.push_back(31557600.0*1.0e09);// 1e9 years

      part->colour.push_back("red");
      part->colour.push_back("orange");
      part->colour.push_back("yellow");
      part->colour.push_back("green");
      part->colour.push_back("cyan");
      part->colour.push_back("blue");
      part->colour.push_back("magenta");
      part->colour.push_back("black");
    }
  //Isomer energy
  else if (draw->choice == "e")
    {
      part->value.push_back(20.0);
      part->value.push_back(50.0);
      part->value.push_back(100.0);
      part->value.push_back(500.0);
      part->value.push_back(1000.0);

      part->colour.push_back("red");
      part->colour.push_back("orange");
      part->colour.push_back("yellow");
      part->colour.push_back("green");
      part->colour.push_back("blue");
      part->colour.push_back("magenta");
      // For an empty square, this should always be last.
      part->colour.push_back("white");
    }
}

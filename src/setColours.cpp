#include "functions.h"

void setColours(std::vector<std::string> &kcol, std::vector<float> &n, inputs *draw)
{
  //Mass excess
  if (draw->choice == "a")
    {
      n.at(0) =   3.0;
      n.at(1) =  10.0;
      n.at(2) =  20.0;
      n.at(3) =  50.0;
      n.at(4) = 100.0;

      kcol.at(0) = "black";
      kcol.at(1) = "magenta";
      kcol.at(2) = "blue";
      kcol.at(3) = "cyan";
      kcol.at(4) = "green";
      kcol.at(5) = "yellow";
      kcol.at(6) = "orange";
      kcol.at(7) = "red";
    }
  //Relative error on mass excess
  else if (draw->choice == "b")
    {
      n.at(0) = 2.0e-05;
      n.at(1) = 1.0e-04;
      n.at(2) = 5.0e-04;
      n.at(3) = 1.0e-03;
      n.at(4) = 5.0e-03;

      kcol.at(0) = "black";
      kcol.at(1) = "blue";
      kcol.at(2) = "green";
      kcol.at(3) = "yellow";
      kcol.at(4) = "orange";
      kcol.at(5) = "red";
    }
  //Decay method
  else if (draw->choice == "c")
    {
      kcol.at(0)  = "black";
      kcol.at(1)  = "yellow";
      kcol.at(2)  = "blue";
      kcol.at(3)  = "red";
      kcol.at(4)  = "magenta";
      kcol.at(5)  = "cyan";
      kcol.at(6)  = "darkgreen";
      kcol.at(7)  = "navyblue";
      kcol.at(8)  = "purple";
      kcol.at(9)  = "green";
      kcol.at(10) = "orange";
    }
  //Decay half-life
  else if (draw->choice == "d")
    {
      //-Use units of seconds
      n.at(0) =        0.1;       // 0.1 second
      n.at(1) =        3.0;       // 3 seconds
      n.at(2) =      120.0;       // 2 mins
      n.at(3) =     3600.0;       // 1 hour
      n.at(4) =    86400.0;       // 1 day
      n.at(5) = 31557600.0;       // 1 year
      n.at(6) = 31557600.0*1.0e09;// 1e9 years

      kcol.at(0) = "red";
      kcol.at(1) = "orange";
      kcol.at(2) = "yellow";
      kcol.at(3) = "green";
      kcol.at(4) = "cyan";
      kcol.at(5) = "blue";
      kcol.at(6) = "magenta";
      kcol.at(7) = "black";
    }
  //Isomer energy
  else if (draw->choice == "e")
    {
      n.at(0) =   20.0;
      n.at(1) =   50.0;
      n.at(2) =  100.0;
      n.at(3) =  500.0;
      n.at(4) = 1000.0;

      kcol.at(0) = "red";
      kcol.at(1) = "orange";
      kcol.at(2) = "yellow";
      kcol.at(3) = "green";
      kcol.at(4) = "blue";
      kcol.at(5) = "magenta";
    }
      
}

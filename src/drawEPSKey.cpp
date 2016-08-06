#include "functions.h"

void drawEPSKey(const inputs *draw,
                std::ofstream &outFile,
                const partition *part
                )
{
  std::vector<std::string> keyString(part->draw.size());

  std::cout << "Drawing the key ";

  outFile << "\n%-------\n"
          << "%- Key -\n"
          << "%-------\n"
          << "/ResetWidth {/StringLength 0 def} def\n"
          << "/TotalWidth {dup stringwidth pop StringLength add /StringLength exch def} def\n"
          << "/KeyWidth 0 def\n"
          << "/TestWidth {StringLength KeyWidth gt {/KeyWidth StringLength def} if} def\n\n"
          << "%-lower left corner of the key-\n";

  if ( draw->section == "a" || (draw->Zmax-draw->Zmin) == MAX_Z )
    {
      std::vector< std::pair<size_t, size_t> > fullChartKeyPosition =
        {
          {15, 75},
          {12, 81},
          { 9, 60},
          { 9, 75},
          {15, 75}
        };

      int index = 0;

           if ( draw->choice == "a" ) index = 0;
      else if ( draw->choice == "b" ) index = 1;
      else if ( draw->choice == "c" ) index = 2;
      else if ( draw->choice == "d" ) index = 3;
      else if ( draw->choice == "e" ) index = 4;

      outFile << fullChartKeyPosition[index].first  << " "
              << fullChartKeyPosition[index].second << " translate\n";
    }
  else
    {
      double yOffset = 0.0;

      //The value of 8 is aesthetic and is the border between vertically centering the key,
      //or vertically centering the chart.
      if ( (draw->Zmax - draw->Zmin) > 8 )
        {
          yOffset = 0.5*( (draw->Zmax - draw->Zmin + 1.0) - draw->key_height*draw->key_scale );
        }

      outFile << (draw->Nmax - draw->Nmin + 2) << " "
              << yOffset                       << " translate\n";
    }

  outFile << draw->key_scale << " dup scale\n" << std::endl;

  if ( draw->choice == "b" )
    {
      outFile << "\n/exponent{\n"
              << "/e1 ed\n"
              << "/e2 ed\n"
              << "1 TR e2 2 string cvs TotalWidth sh\n"
              << "0.75 TR (x) TotalWidth sh\n"
              << "1 TR (10) TotalWidth sh\n"
              << "gs\n"
              << "0.75 TR\n"
              << "0 0.4 rmoveto e1 2 string cvs TotalWidth sh\n"
              << "gr\n"
              << "} def\n\n"
              << "/printUnit{\n"
              << "1 TR (   < ) TotalWidth sh\n"
              << "1 S (d) TotalWidth sh\n"
              << "1 TR (m/m < ) TotalWidth sh\n"
              << "} def\n" << std::endl;
    }
  else if ( draw->choice == "d" )
    {
      outFile << "\n/printUnit{gs\n"
              << "1 S (t) sh\n"
              << "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
              << "gr} def\n" << std::endl;
    }

  //-Construct an array of strings with the text for each part of the key
  setEPSKeyText(draw,part,keyString);

  double relativeKeyPosition = 0.5;
  for ( size_t i=0; i<part->draw.size(); ++i )
    {
      if ( part->draw[part->draw.size()-(i+1)] )
        {
          //The max size of part->colour is 12
          outFile << "0 " << part->colour[11-i] << " 0.5 " << relativeKeyPosition << " curve Nucleus\n"
                  << "2.5 " << relativeKeyPosition+0.2 << " m ResetWidth\n"
                  << keyString[11-i];

          relativeKeyPosition += 1.5;
        }
    }

  //-Draw a dynamically sized box around the key
  outFile << "\n"
          << "%Draw a box around the key\n"
          << "0.1 u div sl\n"
          << "0 0 m\n"
          << "KeyWidth 3 add 0 rl\n"
          << "0 " << draw->key_height << " rl\n"
          << "KeyWidth 3 add neg 0 rl\n"
          << "closepath\n"
          << "st\n" << std::endl;

  std::cout << "- done" << std::endl;
}

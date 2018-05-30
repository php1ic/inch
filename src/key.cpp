#include "key.hpp"

void Key::setScale(const std::unique_ptr<inputs> &draw, const std::unique_ptr<partition> &part) const
{
  if ( !draw->key )
    {
      scale = 0.0;
      return;
    }

  /// Set the key height by checking how many partition types are used
  std::for_each(std::cbegin(part->draw), std::cend(part->draw),
                [&](const bool val)
                {
		  height += static_cast<double>(val)*1.5;
                }
                );

  /// We don't want the key to shrink below a certain size.
  scale = ( (draw->Zmax - draw->Zmin) > KEY_YOFFSET )
    ? (draw->Zmax - draw->Zmin)/height
    : KEY_YOFFSET/height;

  /// Nor do we want it to be larger than a certain size.
  if (   scale > 3.0
      || draw->section == "a"
      || (draw->Zmax - draw->Zmin) == MAX_Z)
    {
      scale = 3.0;
    }
}


void Key::EPSSetup(std::ofstream &outFile) const
{
  std::cout << "Drawing the key ";

  outFile << "\n%-------\n"
          << "%- Key -\n"
          << "%-------\n"
          << "/ResetWidth {/StringLength 0 def} def\n"
          << "/TotalWidth {dup stringwidth pop StringLength add /StringLength exch def} def\n"
          << "/KeyWidth 0 def\n"
          << "/TestWidth {StringLength KeyWidth gt {/KeyWidth StringLength def} if} def\n\n"
          << "%-lower left corner of the key-\n";
}


void Key::EPSPlaceKey(std::ofstream &outFile, const std::unique_ptr<inputs> &draw) const
{
  if ( draw->section == "a" || (draw->Zmax-draw->Zmin) == MAX_Z )
    {
      int index = 0;

      if ( draw->choice == "a" )
        {
          index = 0;
        }
      else if ( draw->choice == "b" )
        {
          index = 1;
        }
      else if ( draw->choice == "c" )
        {
          index = 2;
        }
      else if ( draw->choice == "d" )
        {
          index = 3;
        }
      else if ( draw->choice == "e" )
        {
          index = 4;
        }

      outFile << fullChartKeyPosition[index].first  << " "
              << fullChartKeyPosition[index].second << " translate\n";
    }
  else
    {
      double yOffset = 0.0;

      /// The value of KEY_YOFFSET is aesthetic and is the border between
      /// vertically centering the key, or vertically centering the chart.
      if ( (draw->Zmax - draw->Zmin) >= KEY_YOFFSET )
        {
          yOffset = 0.5*( (draw->Zmax - draw->Zmin + 1.0) - height*scale );
        }

      outFile << (draw->Nmax - draw->Nmin + 2) << " "
              << yOffset                       << " translate\n";
    }

  outFile << scale << " dup scale\n" << std::endl;
}


void Key::EPSAdditionalFunctions(std::ofstream &outFile, const std::unique_ptr<inputs> &draw) const
{
  if ( draw->choice == "b" )
    {
      outFile << "\n/exponent{\n"
              << "/e1 ed\n"
              << "/e2 ed\n"
              << "1 TR e2 5 string cvs TotalWidth sh\n"
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
}


void Key::EPSSurroundingBox(std::ofstream &outFile) const
{
  /// Draw a dynamically sized box around the key
  outFile << "\n"
          << "%Draw a box around the key\n"
          << "0.1 u div sl\n"
          << "0 0 m\n"
          << "KeyWidth 3 add 0 rl\n"
          << "0 " << height << " rl\n"
          << "KeyWidth 3 add neg 0 rl\n"
          << "closepath\n"
          << "st\n" << std::endl;

  std::cout << "- done" << std::endl;
}


void Key::EPSSetText(const std::unique_ptr<inputs> &draw, const std::unique_ptr<partition> &part) const
{
  textStrings.resize(part->draw.size());

  const Converter convert;

  auto text = std::begin(textStrings);

  if ( draw->choice == "a" )
    {
      auto low = convert.FloatToNdp(part->value[0], 1);
      auto high = convert.FloatToNdp(part->value[1], 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (  ";
      *text += low;
      *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(textStrings)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.FloatToNdp(part->value[index+1], 1);
          *text = "1 TR (";
          *text += low;
          *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
          *text += high;
          *text += " keV) TotalWidth sh TestWidth\n";
          std::advance(text,1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "b" )
    {
      auto low  = convert.FloatToExponent(part->value[0]);
      auto high = convert.FloatToExponent(part->value[1]);

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n";
      *text += std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      *text = std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent printUnit ";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(textStrings)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.FloatToExponent(part->value[index+1]);

          *text = std::get<0>(low);
          *text += " ";
          *text += std::get<1>(low);;
          *text += std::get<2>(low);
          *text += " exponent printUnit ";
          *text += std::get<0>(high);
          *text += " ";
          *text += std::get<1>(high);
          *text += std::get<2>(high);
          *text += " exponent TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
    }
  else if ( draw->choice == "c" )
    {
      *text = "1 TR (Stable) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Alpha) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Unknown) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Electron Capture) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "d" )
    {
      std::string low = convert.SecondsToHuman(part->value[0]);
      std::string high = convert.SecondsToHuman(part->value[1]);

      *text = "printUnit 1 TR (     < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < ) TotalWidth sh printUnit\n(     < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(textStrings)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.SecondsToHuman(part->value[index+1]);

          *text = "1 TR (";
          *text += low;
          *text += " < ) TotalWidth sh printUnit\n(     < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "printUnit 1 TR (     > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "e" )
    {
      auto low  = convert.IsomerEnergyToHuman(part->value[0]);
      auto high = convert.IsomerEnergyToHuman(part->value[1]);

      *text = "1 TR (E < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < E < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 2;
      while ( (text - std::begin(textStrings)) < static_cast<int>(part->colour.size() - 2) )
        {
          low = high;
          high = convert.IsomerEnergyToHuman(part->value[index]);

          *text = "1 TR (";
          *text += low;
          *text +=" < E < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 TR (E > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }
}


void Key::EPSWrite(std::ofstream &outFile, const std::unique_ptr<partition> &part) const
{
  /// Only draw the parts of the key required
  double yPos = 0.5;
  for ( auto it = std::crbegin(part->draw); it != std::crend(part->draw); ++it )
    {
      if ( *it )
        {
          auto jump = std::crend(part->draw) - (it+1);

          outFile << "0 " << *std::next(std::cbegin(part->colour), jump) << " 0.5 " << yPos << " curve Nucleus\n"
                  << "2.5 " << yPos + 0.2 << " m ResetWidth\n"
                  << *std::next(std::cbegin(textStrings), jump);

          yPos += 1.5;
        }
    }
}

#include "functions.hpp"

void drawNuclei(std::vector<Nuclide> &in,
                const std::unique_ptr<inputs> &draw,
                std::ostream &outFile)
{
  for ( auto it : in )
    {
      if ( it.show == 1 )
        {
          if ( draw->filetype == FileType::EPS )
            {
              /// Set how the shape representing the isotope is displayed
              const int isotope_display =
                [&]()
                {
                  return ( draw->choice == "e" && it.decay == "stable" ) ?
                    1 :
                    it.own ? 8 : 0;
                }();

              /// Set the text, if it is displayed
              const std::string writing_colour =
                [&]()
                {
                  if ( draw->write_isotope )
                    {
                      std::string text_colour {"black"};
                      /// If the square is coloured black, change text colour to white
                      if ( it.colour == "black" )
                        {
                          /// but if it's user defined, use red
                          text_colour = it.own ? " red" : " white";
                        }
                      else if ( draw->choice=="e" && it.decay=="stable" )
                        {
                          text_colour = "white";
                        }

                      return text_colour + " (" + it.symbol + ") (" + std::to_string(it.A) + ")";
                    }

                  return std::string("");
                }();

              /// Everything is set, draw the isotope
              /// Add comment in the eps file with isotope ID
              /// This is for easy navigation if manually altering the file
              outFile << "%" << it.A << it.symbol << "\n"
                      << isotope_display
                      << " " << writing_colour
                      << " " << it.colour
                      << " " << it.N-draw->Nmin
                      << " " << it.Z-draw->Zmin
                      << " curve Nucleus\n";
            }
          else if ( draw->filetype == FileType::SVG )
            {
              outFile << "<!--" << it.A << it.symbol << "-->\n"
                      << R"(<g transform="translate()" << it.N-draw->Nmin << " " << draw->Zmax-it.Z << R"lit()"> )lit"
                      << R"(<use xlink:href="#)" << it.colour << R"(Nucleus"/></g>)" << std::endl;
              //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << it.symbol << "</text> "
              //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << it.A << "</text></g>" << std::endl;
            }
          else if ( draw->filetype == FileType::TIKZ )
            {
              outFile << "%" << it.A << it.symbol << "\n"
                      << R"(\nucleus{)" << it.colour << "}{"
                      << it.N << "}{"
                      << it.Z << "}{"
                      << it.A << "}{"
                      << it.symbol << "}" << std::endl;
            }
        }
      else if ( it.show == 2 )
        {
          if ( it.decay == "stable" )
            {
              it.colour = "black";
            }

          if ( draw->filetype == FileType::EPS )
            {
              /// Set the text, if it is displayed
              const std::string writing_colour =
                [&]()
                {
                  if ( draw->write_isotope )
                    {
                      /// If the square is coloured black change the text to white
                      /// unless it a user isotope, in which case red
                      const std::string text_colour = ( it.colour == "black" ) ?
                        it.own ? "red" : "white" :
                        "black";

                      return text_colour + " (" + it.symbol + ") (" + std::to_string(it.A) + ")";
                    }

                  return std::string("");
                }();

              outFile << "%" << it.A << it.symbol << "\n"
                      << "0"
                      << " " << writing_colour
                      << " " << it.colour
                      << " " << it.N-draw->Nmin
                      << " " << it.Z-draw->Zmin
                      << " curve Nucleus" << std::endl;
            }
          else if ( draw->filetype == FileType::SVG )
            {
            }
          else if ( draw->filetype == FileType::TIKZ )
            {
            }
        }
    }
}

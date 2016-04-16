#include "functions.h"

void drawNuclei(std::vector<Nuclide> &in,
                const inputs *draw,
                std::ostream &outFile)
{
  for ( auto it=in.begin(); it!=in.end(); ++it )
    {
      if ( it->show == 1 )
        {
          if ( draw->file_type == 0 )
            {
              outFile << "%" << it->A << draw->convertZToSymbol(it->Z) << "\n";

              if (draw->choice=="e"
                  && it!=in.begin()
                  && (it-1)->N==it->N
                  && (it-1)->Z==it->Z
                  && (it-1)->st==0
                  && (it-1)->decay=="stable")
                {
                  outFile << "1";
                }
              else
                {
                  //-If user specified nuclei are to be drawn,
                  //-mark with a half square along the diagonal
                  outFile << (it->own ? "8" : "0");
                }

              if ( draw->write_isotope )
                {
                  //-If the square is coloured black, change text colour to white
                  if ( it->colour == "black" )
                    {
                      //-If the square is black and marked as an own nuclei,
                      //-change text colour to red
                      outFile << (it->own ? " red" : " white");
                    }
                  else if (draw->choice=="e"
                           && it!=in.begin()
                           && (it-1)->N==it->N
                           && (it-1)->Z==it->Z
                           && (it-1)->st==0
                           && (it-1)->decay=="stable")
                    {
                      outFile << " white";
                    }
                  else
                    {
                      outFile << " black";
                    }

                  outFile << " (" << draw->convertZToSymbol(it->Z) << ") (" << it->A << ")";
                }

              outFile << " " << it->colour
                      << " " << it->N-draw->Nmin
                      << " " << it->Z-draw->Zmin
                      << " curve Nucleus" << std::endl;
            }
          else if ( draw->file_type == 1 )
            {
              outFile << "<!--" << it->A << draw->convertZToSymbol(it->Z) << "-->\n";

              outFile << "<g transform=\"translate(" << it->N-draw->Nmin << " " << draw->Zmax-it->Z << ")\"> "
                      << "<use xlink:href=\"#" << it->colour << "Nucleus\"/></g>" << std::endl;
              //<< "<text class=\"MidSymbol Black\" dx=\"0.5\" dy=\"0.80\">" << draw->convertZToSymbol(it->Z) << "</text> "
              //<< "<text class=\"MidNumber Black\" dx=\"0.5\" dy=\"0.35\">" << it->A << "</text></g>" << std::endl;
            }
          else if ( draw->file_type == 2 )
            {
              outFile << "%" << it->A << draw->convertZToSymbol(it->Z) << "\n";

              outFile << "\\nucleus{" << it->colour << "}{"
                      << it->N << "}{"
                      << it->Z << "}{"
                      << it->A << "}{"
                      << draw->convertZToSymbol(it->Z) << "}" << std::endl;
            }
        }
      else if ( it->show == 2 )
        {
          if ( it->decay=="stable" )
            {
              it->colour="black";
            }

          if ( draw->file_type == 0 )
            {
              outFile << "%" << it->A << draw->convertZToSymbol(it->Z) << "\n"
                      << "0";

              if ( draw->write_isotope )
                {
                  //-If the square is coloured black, change text colour to white
                  if ( it->colour == "black" )
                    {
                      //-If the square is black and marked as an own nuclei,
                      //-change text colour to red
                      outFile << (it->own ? " red" : " white");
                    }
                  else
                    {
                      outFile << " black";
                    }

                  outFile << " (" << draw->convertZToSymbol(it->Z) << ") (" << it->A << ")";
                }

              outFile << " " << it->colour
                      << " " << it->N-draw->Nmin
                      << " " << it->Z-draw->Zmin
                      << " curve Nucleus" << std::endl;
            }
          else if ( draw->file_type == 1 )
            {
            }
          else if ( draw->file_type == 2 )
            {
            }
        }
    }
}

#include "include/functions.h"

void constructOutputFilename(inputs *draw)
{
  std::string name=draw->outfile;

  //-Remove the extension if given
  if (   draw->outfile.find(".") == draw->outfile.length()-4
      && draw->outfile.find(".") != std::string::npos
      )
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      draw->outfile.erase(draw->outfile.rfind("."),4);
    }

  //-Remove the CWD if given
  if ( draw->outfile.find(draw->pwd) != std::string::npos )
    {
      std::cout << "\nThe current working directory is added\n";

      draw->outfile.erase(0,draw->outfile.rfind("/")+1);
    }

  //-Check input file is not a directory.
  if (   draw->outfile.empty()
      || draw->outfile.at(draw->outfile.length()-1) == '/'
      )
    {
      std::cout << "\nERROR: " << name << " is a directory, can't use that as a file name\n" << std::endl;
      exit(-1);
    }
  else
    {
      std::cout << "Using \"" << draw->outfile << "\" as the base of the file name." << std::endl;

      //-Add the necessary extension
      if (draw->file_type == 0)
	draw->outfile.append(".eps");
      else if (draw->file_type == 1)
	draw->outfile.append(".svg");
      else if (draw->file_type == 2)
	draw->outfile.append(".tex");
    }
}

#include "functions.h"

bool validateFileType(inputs *draw,
                      const std::string &fileType
                      )
{
  //0 - eps
  //1 - svg
  //2 - tikz

  if ( fileType == "eps" || fileType == "0" )
    {
      draw->file_type = 0;
    }
  else if ( fileType == "svg" || fileType == "1" )
    {
      draw->file_type = 1;
    }
  else if ( fileType == "tikz" || fileType == "2" )
    {
      draw->file_type = 2;
    }
  else
    {
      draw->file_type = 0;
      return false;
    }

  return true;
}

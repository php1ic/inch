#include "include/functions.h"

void constructFilePaths(inputs *draw)
{
  draw->pwd = getenv("PWD");
  draw->pwd.append("/");

  draw->path = LOCAL_PATH;
  draw->path.append("/");

  draw->path = draw->path.append("data_files/");

  std::cout << "\nSetting the path to the required files as:\n"
	    << draw->path << "\n" << std::endl;
}

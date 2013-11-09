#include "include/functions.h"

void createChart(inputs *draw,
		 partition *part,
		 std::vector<Nuclide> &nuc,
		 std::vector<std::string> &arguments,
		 bool &inputfile)
{
  printBanner(draw);

  printUsage(arguments);

  constructFilePaths(draw);

  constructFullyQualifiedPaths(draw);

  //-Read mass table -
  if (readNUBASE(draw->mass_table_NUBASE,nuc))
    {
      std::cout << "Nuclear data has not been read, exiting..." << std::endl;
      exit(-1);
    }

  if (draw->AME)
    if (readAME(draw->mass_table_AME,nuc))
      std::cout << "Updated values from AME were not read." << std::endl;

  //-Read user defined nuclei -
  if (draw->own_nuclei)
    {
      if (readOWN(draw->my_nuclei,nuc))
	std::cout << "User defined nuclei have not been read." << std::endl;
    }
  else
    std::cout << "Not drawing any user selected nuclei" << std::endl;

  //-Check and validate arguments -
  validateInputArguments(nuc,draw,arguments,inputfile);

  //-Ask how the chart should be displayed -
  //--Only if no inputfile is given --------
  if (!inputfile)
    displaySection(nuc,draw);

  //-Ouput details about what will be drawn.
  printSelection(draw);

  //-Define what colours and values will be used to differentiate the nuclei.
  setColours(part,draw);

  //-Draw the nuclei in the selected range.
  showNuclei(nuc,part,draw);

  //-Use the drawn nuclei to decide how large the key should be.
  setKeyScale(draw,part);

  //-Set the size of the canvas
  setCanvasSize(draw);

  //-Write the chart -
  drawChart(nuc,draw,part);

  //-Write chart parameters to file that can be reused -
  writeOptionFile(draw);
}

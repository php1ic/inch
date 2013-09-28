#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <pwd.h>
#include <sys/stat.h>
#include <algorithm>

#include "nuclide.h"
#include "inputs.h"
#include "partition.h"

double errorQuadrature(const int, ...);

int convertSymbolToZ(const std::string &);

std::string convertZToSymbol(const int &);

void constructFilePath(inputs *);

void constructOutputFilename(inputs *, const std::string &);

void convertFloatToExponent(const float &, std::vector<std::string> &);

void convertSecondsToHuman(const float &, std::string &);

void createDriplineFile(const std::vector<Nuclide> &, const inputs *, const int &);

void createEPSKey(inputs *, std::ofstream &, const std::vector<std::string> &, partition *);

void createEPSProlog(const inputs *, std::ofstream &);

void createSVGProlog(const inputs *, std::ofstream &);

void createTIKZProlog(const inputs *, std::ofstream &);

void displaySection(std::vector<Nuclide> &, inputs *);

void drawEPSDoubleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawEPSGrid(const inputs *, std::ofstream &);

void drawEPSKey(inputs *, std::ofstream &, partition *);

void drawEPSMagicNumbers(const inputs *, std::ofstream &);

void drawEPSRprocess(inputs *, std::ofstream &, const bool);

void drawEPSSingleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawNuclei(std::vector<Nuclide> &, const inputs *, std::ostream &);

void readAME(const std::string &, std::vector<Nuclide> &);

void readNUBASE(const std::string &, std::vector<Nuclide> &);

void readOWN(const std::string &, std::vector<Nuclide> &);

void setColours(partition *, const inputs *);

void setExtreme(const std::string , inputs *);

void setIsomerUnit(const float &, std::string &);

void setKeyScale(inputs *, const partition *);

void setNeutronLimits(const std::vector<Nuclide> &, inputs *);

void showNuclei(std::vector<Nuclide> &, partition *, const inputs *);

void validateInputArguments(const std::vector<Nuclide> &, inputs *, const std::vector<std::string> &, bool &, const std:: string &, int &);

void validateInputFile(const std::vector<Nuclide> &, inputs *, const std::string &, bool &);

void validateOutputFile(inputs *, const std::string &, const std::string &);

void writeEPS(std::vector<Nuclide> &, inputs *, partition *);

void writeSVG(std::vector<Nuclide> &, inputs *, partition *);

void writeTIKZ(std::vector<Nuclide> &, inputs *, partition *);

#endif

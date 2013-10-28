#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdarg>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <sys/stat.h>
#include <algorithm>

#include "nuclide.h"
#include "inputs.h"
#include "partition.h"

double errorQuadrature(const int, ...);

bool checkFileExists(const std::string &);

int convertSymbolToZ(const std::string &);

std::string convertZToSymbol(const int &);

void constructFilePaths(inputs *);

void constructFullyQualifiedPaths(inputs *);

void constructOutputFilename(inputs *);

void convertFloatToExponent(const float &, std::vector<std::string> &);

void convertSecondsToHuman(const float &, std::string &);

void createChart(inputs *, partition *, std::vector<Nuclide> &, std::vector<std::string> &, bool &);

void createDriplineFile(const std::vector<Nuclide> &, const inputs *, const int &);

void createEPSKey(inputs *, std::ofstream &, const std::vector<std::string> &, partition *);

void createEPSProlog(const inputs *, std::ofstream &);

void createSVGProlog(const inputs *, std::ofstream &);

void createTIKZProlog(const inputs *, std::ofstream &);

void displaySection(std::vector<Nuclide> &, inputs *);

void drawChart(std::vector<Nuclide> &, inputs *, partition *);

void drawEPSDoubleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawEPSGrid(const inputs *, std::ofstream &);

void drawEPSKey(inputs *, std::ofstream &, partition *);

void drawEPSMagicNumbers(const inputs *, std::ofstream &);

void drawEPSRprocess(inputs *, std::ofstream &, const bool);

void drawEPSSingleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawNuclei(std::vector<Nuclide> &, const inputs *, std::ostream &);

void printBanner(inputs *);

void printSelection(inputs *);

void printUsage(const std::vector<std::string> &);

bool readAME(const std::string &, std::vector<Nuclide> &);

bool readNUBASE(const std::string &, std::vector<Nuclide> &);

bool readOWN(const std::string &, std::vector<Nuclide> &);

void setCanvasSize(inputs *);

void setColours(partition *, const inputs *);

void setExtreme(const std::string &, inputs *);

void setIsomerUnit(const float &, std::string &);

void setKeyScale(inputs *, const partition *);

void setNeutronLimits(const std::vector<Nuclide> &, inputs *);

void showNuclei(std::vector<Nuclide> &, partition *, const inputs *);

void validateInputArguments(const std::vector<Nuclide> &, inputs *, const std::vector<std::string> &, bool &);

void validateInputFile(const std::vector<Nuclide> &, inputs *, const std::string &, bool &);

void validateOutputFile(inputs *, const std::string &);

void writeEPS(std::vector<Nuclide> &, inputs *, partition *);

void writeOptionFile(inputs *);

void writeSVG(std::vector<Nuclide> &, inputs *, partition *);

void writeTIKZ(std::vector<Nuclide> &, inputs *, partition *);

#endif

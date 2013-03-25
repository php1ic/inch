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

int convertSymbolToZ(const std::string &);

double errorQuadrature(const int, ...);

std::string convertZToSymbol(const int &);

void readAME(const std::string &, std::vector<Nuclide> &);

void readNUBASE(const std::string &, std::vector<Nuclide> &);

void readOWN(const std::string &, std::vector<Nuclide> &);

void writeEPS(const std::vector<Nuclide> &, inputs *);

void writeSVG(const std::vector<Nuclide> &, inputs *);

void writeTIKZ(const std::vector<Nuclide> &, inputs *);

void convertFloatToExponent(const float &, std::vector<std::string> &);

void setIsomerUnit(const float &, std::string &);

void convertSecondsToHuman(const float &, std::string &);

void displaySection(std::vector<Nuclide> &, inputs *);

void setExtreme(const std::string , inputs *);

void setNeutronLimits(const std::vector<Nuclide> &, inputs *);

void setColours(std::vector<std::string> &, std::vector<float> &, const inputs *);

void drawNuclei(const std::vector<Nuclide> &, const std::vector<std::string> &, const std::vector<float> &, std::vector<bool> &, const inputs *, std::ofstream &);

void drawEPSRprocess(inputs *, std::ofstream &, const bool);

void drawEPSGrid(const inputs *, std::ofstream &);

void createEPSProlog(const inputs *, std::ofstream &);

void createSVGProlog(const inputs *, std::ofstream &);

void createTIKZProlog(const inputs *, std::ofstream &);

void drawEPSMagicNumbers(const inputs *, std::ofstream &);

void drawEPSSingleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawEPSDoubleDriplines(const std::vector<Nuclide> &, inputs *, std::ofstream &);

void drawEPSKey(const inputs *, std::ofstream &, float &, float &, const std::vector<std::string> &, const std::vector<bool> &, const std::vector<float> &);

void createDriplineFile(const std::vector<Nuclide> &, const inputs *, const int &);

void setKeyScale(const inputs *, float &, float &, const std::vector<bool> &);

void createEPSKey(const inputs *, std::ofstream &,const float &, const float &, const std::vector<std::string> &, const std::vector<std::string> &, const std::vector<bool> &);

void constructOutputFilename(inputs *, const std::string &);

void validateInputFile(const std::vector<Nuclide> &, inputs *, const std::string &, bool &);

void validateOutputFile(inputs *, const std::string &, const std::string &);

void validateInputArguments(const std::vector<Nuclide> &, inputs *, const std::vector<std::string> &, bool &, const std:: string &, int &);
#endif

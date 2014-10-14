#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <iomanip>
#include <sys/stat.h>

#include "nuclide.h"
#include "inputs.h"
#include "partition.h"


bool checkFileExists(const std::string &);

void constructChart(inputs *, partition *, std::vector<Nuclide> &);

void convertFloatToExponent(const double &, std::vector<std::string> &);

void convertIsomerEnergyToHuman(const double &, std::string &);

void convertSecondsToHuman(const double &, std::string &);

int countLinesInFile(std::ifstream &);

void createDriplineFile(const std::vector<Nuclide> &, const inputs *, const int &);

void createEPSProlog(const inputs *, std::ofstream &);

void createSVGProlog(const inputs *, std::ofstream &);

void createTIKZProlog(const inputs *, std::ofstream &);

void displaySection(std::vector<Nuclide> &, inputs *);

void drawEPSDoubleDriplines(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawEPSDoubleProtonDripline(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawEPSDoubleNeutronDripline(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawEPSDripline(const std::vector<Nuclide> &, const inputs *, std::ofstream &, const int &);

void drawEPSGrid(const inputs *, std::ofstream &);

void drawEPSKey(const inputs *, std::ofstream &, const partition *);

void drawEPSMagicNumbers(const inputs *, std::ofstream &);

void drawEPSRprocess(inputs *, std::ofstream &, const bool);

void drawEPSSingleDriplines(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawEPSSingleProtonDripline(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawEPSSingleNeutronDripline(const std::vector<Nuclide> &, const inputs *, std::ofstream &);

void drawNuclei(std::vector<Nuclide> &, const inputs *, std::ostream &);

void populateInternalMassTable(inputs *, std::vector<Nuclide> &);

bool readAME(const std::string &, std::vector<Nuclide> &);

bool readNUBASE(const std::string &, std::vector<Nuclide> &);

std::map<std::string, std::string> readOptionFile(const std::string &);

bool readOWN(const std::string &, std::vector<Nuclide> &);

bool readRProcessData(inputs *);

void setColours(partition *, const inputs *);

void setEPSKeyText(const inputs *, const partition *, std::vector<std::string> &);

void setIsotopeAttributes(std::vector<Nuclide> &, partition *, const inputs *);

void setKeyScale(inputs *, const partition *);

void setNeutronLimits(const std::vector<Nuclide> &, inputs *);

bool validateFileType(inputs *, const std::string &);

bool validateInputArguments(const std::vector<Nuclide> &, inputs *, const std::vector<std::string> &);

bool validateInputFile(const std::vector<Nuclide> &, inputs *, const std::string &);

bool validateOutputFile(inputs *, const std::string &);

void writeChart(std::vector<Nuclide> &, inputs *, partition *);

void writeEPS(std::vector<Nuclide> &, inputs *, partition *);

void writeSVG(std::vector<Nuclide> &, inputs *);

void writeTIKZ(std::vector<Nuclide> &, inputs *);

#endif  //FUNCTIONS_H

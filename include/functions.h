#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sstream>
#include <iomanip>
#include <sys/stat.h>

#include "nuclide.h"
#include "inputs.h"
#include "partition.h"


bool checkFileExists(const std::string &file);

void constructChart(inputs *draw, partition *partition, std::vector<Nuclide> &nuc);

void convertFloatToExponent(const double in, std::vector<std::string> &out);

void convertIsomerEnergyToHuman(const double in, std::string &out);

void convertSecondsToHuman(const double in, std::string &out);

void createDriplineFile(const inputs *draw, const double meN, const double meP, const int np);

void createEPSProlog(const inputs *draw, std::ofstream &outFile);

void createSVGProlog(const inputs *draw, std::ofstream &outFile);

void createTIKZProlog(const inputs *draw, std::ofstream &outFile);

void displaySection(std::vector<Nuclide> &in , inputs *draw);

void drawEPSDoubleDriplines(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawEPSDoubleProtonDripline(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawEPSDoubleNeutronDripline(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawEPSDripline(const double meN, const double meP, const inputs *draw, std::ofstream &outFile, const int np);

void drawEPSGrid(const inputs *draw, std::ofstream &outFile);

void drawEPSKey(const inputs *draw, std::ofstream &outFile, const partition *part);

void drawEPSMagicNumbers(const inputs *draw, std::ofstream &outFile);

void drawEPSRprocess(inputs *draw, std::ofstream &outFile, const bool shaded);

void drawEPSSingleDriplines(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawEPSSingleProtonDripline(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawEPSSingleNeutronDripline(const double meN, const double meP, const inputs *draw, std::ofstream &outFile);

void drawNuclei(std::vector<Nuclide> &in, const inputs *draw, std::ostream &outFile);

void populateInternalMassTable(inputs * draw, std::vector<Nuclide> &nuc);

bool readAME(const std::string &table, std::vector<Nuclide> &nuc);

bool readNUBASE(const std::string &table, const int year, std::vector<Nuclide> &nuc);

bool readOWN(const std::string &myNuclei, std::vector<Nuclide> &nuc);

bool readRProcessData(inputs *draw);

void setColours(partition *part, const inputs *draw);

void setEPSKeyText(const inputs *draw, const partition *part, std::vector<std::string> &keyString);

void setIsotopeAttributes(std::vector<Nuclide> &in, partition *part, const inputs *draw);

void setKeyScale(inputs *draw, const partition *part);

void setNeutronLimits(const std::vector<Nuclide> &in, inputs *draw);

bool validateInputFile(const std::vector<Nuclide> &nuc, inputs *draw);

void writeChart(std::vector<Nuclide> &nuc, inputs *draw, partition *part);

void writeEPS(std::vector<Nuclide> &nuc, inputs *draw, partition *part);

void writeSVG(std::vector<Nuclide> &nuc, inputs *draw);

void writeTIKZ(std::vector<Nuclide> &nuc, inputs *draw);

#endif  //FUNCTIONS_H

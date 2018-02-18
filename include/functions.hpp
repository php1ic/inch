#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#include <memory>

#include "nuclide.hpp"
#include "inputs.hpp"
#include "partition.hpp"
#include "massTable.hpp"


bool checkFileExists(const std::string &file);

void constructChart(std::unique_ptr<inputs> &draw, std::unique_ptr<partition> &part, std::vector<Nuclide> &nuc);

std::pair<std::string, std::string> convertFloatToExponent(const double in);

std::string convertIsomerEnergyToHuman(const double in);

std::string convertSecondsToHuman(const double in);

void createDriplineFile(const std::unique_ptr<inputs> &draw, const double meN, const double meP, const int np);

void createEPSProlog(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void createSVGProlog(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void createTIKZProlog(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void displaySection(std::vector<Nuclide> &in , std::unique_ptr<inputs> &draw);

void drawEPSDoubleDriplines(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSDoubleProtonDripline(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSDoubleNeutronDripline(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSDripline(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile, const int np);

void drawEPSGrid(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSKey(const std::unique_ptr<inputs> &draw, std::ofstream &outFile, const std::unique_ptr<partition> &part);

void drawEPSMagicNumbers(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSRprocess(std::unique_ptr<inputs> &draw, std::ofstream &outFile, const bool shaded);

void drawEPSSingleDriplines(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSSingleProtonDripline(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSSingleNeutronDripline(const double meN, const double meP, const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawNuclei(std::vector<Nuclide> &in, const std::unique_ptr<inputs> &draw, std::ostream &outFile);

void setColours(std::unique_ptr<partition> &part, const std::unique_ptr<inputs> &draw);

void setEPSKeyText(const std::unique_ptr<inputs> &draw, const std::unique_ptr<partition> &part, std::vector<std::string> &keyString);

void setIsotopeAttributes(std::vector<Nuclide> &in, std::unique_ptr<partition> &part, const std::unique_ptr<inputs> &draw);

void setKeyScale(std::unique_ptr<inputs> &draw, const std::unique_ptr<partition> &part);

void setNeutronLimits(const std::vector<Nuclide> &in, std::unique_ptr<inputs> &draw);

bool validateInputFile(const std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw);

void writeChart(std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw, std::unique_ptr<partition> &part);

void writeEPS(std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw, std::unique_ptr<partition> &part);

void writeSVG(std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw);

void writeTIKZ(std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw);

#endif // FUNCTIONS_HPP

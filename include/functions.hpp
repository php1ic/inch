#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <sstream>
#include <iomanip>
#include <memory>

#include "nuclide.hpp"
#include "inputs.hpp"
#include "partition.hpp"
#include "massTable.hpp"


void constructChart(std::unique_ptr<inputs> &draw, std::unique_ptr<partition> &part, std::vector<Nuclide> &nuc);

void displaySection(std::vector<Nuclide> &in , std::unique_ptr<inputs> &draw);

void drawEPSGrid(const std::unique_ptr<inputs> &draw, std::ofstream &outFile);

void drawEPSKey(const std::unique_ptr<inputs> &draw, std::ofstream &outFile, const std::unique_ptr<partition> &part);

void drawNuclei(std::vector<Nuclide> &in, const std::unique_ptr<inputs> &draw, std::ostream &outFile);

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

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <sstream>
#include <iomanip>
#include <memory>

#include "nuclide.hpp"
#include "inputs.hpp"
#include "partition.hpp"
#include "massTable.hpp"

void displaySection(std::vector<Nuclide> &in , std::unique_ptr<inputs> &draw);

void drawNuclei(std::vector<Nuclide> &in, const std::unique_ptr<inputs> &draw, std::ostream &outFile);

void setIsotopeAttributes(std::vector<Nuclide> &in, std::unique_ptr<Partition> &part, const std::unique_ptr<inputs> &draw);

void setNeutronLimits(const std::vector<Nuclide> &in, std::unique_ptr<inputs> &draw);

bool validateInputFile(const std::vector<Nuclide> &nuc, std::unique_ptr<inputs> &draw);

#endif // FUNCTIONS_HPP

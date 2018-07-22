#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <sstream>
#include <iomanip>
#include <memory>

#include "nuclide.hpp"
#include "inputs.hpp"
#include "partition.hpp"
#include "massTable.hpp"

void setIsotopeAttributes(std::vector<Nuclide> &in, std::unique_ptr<Partition> &part, const std::unique_ptr<inputs> &draw);

#endif // FUNCTIONS_HPP

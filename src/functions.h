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

int convertSymbolToZ(const std::string &el);

double error(int x, ...);

std::string convertZToSymbol(const int &Z);

void readAME(const std::string &table, std::vector<Nuclide> &nuc);

void readNUBASE(const std::string &table, std::vector<Nuclide> &nuc);

void readOWN(const std::string &my_nuclei, std::vector<Nuclide> &nuc);

void writeEPS(std::vector<Nuclide> &in, inputs *draw);

void writeSVG(std::vector<Nuclide> &in, inputs *draw);

void writeTIKZ(std::vector<Nuclide> &in, inputs *draw);

void convertFloatToExponent(const float &in, std::vector<std::string> &out);

void setIsomerUnit(const float &in, std::string &out);

void convertSecondsToHuman(const float &in, std::string &out);

void displaySection(std::vector<Nuclide> &in, inputs *draw);

void setExtreme(std::string limit, inputs *draw);

void setNlimits(std::vector<Nuclide> &in, inputs *draw);

void setColours(std::vector<std::string> &kcol, std::vector<float> &n, inputs *draw);

void drawNuclei(std::vector<Nuclide> &in, std::vector<std::string> &kcol, std::vector<float> &n, std::vector<bool> &k, inputs *draw, std::ofstream &out_file);

void drawRprocess(inputs *draw, std::ofstream &out_file, bool shaded);

void drawMagicNumbers(inputs *draw, std::ofstream &out_file);

void drawSingleDriplines(std::vector<Nuclide> &nuc, inputs *draw, std::ofstream &out_file);

void drawDoubleDriplines(std::vector<Nuclide> &nuc, inputs *draw, std::ofstream &out_file);

void drawKey(inputs *draw, std::ofstream &out_file, float &s, std::vector<std::string> &kcol, std::vector<bool> &k, std::vector<float> &n);

void createDriplineFile(std::vector<Nuclide> &nuc, inputs *draw, int np);
#endif

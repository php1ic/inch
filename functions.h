#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cstdlib>
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

#include "nuclide.h"
#include "inputs.h"

unsigned short el_z(const std::string &el);

std::string z_el(const unsigned short &Z);

void read_AME(const std::string &table, std::vector<Nuclide> &nuc);

void read_NUBASE(const std::string &table, std::vector<Nuclide> &nuc);

void read_OWN(const std::string &my_nuclei, std::vector<Nuclide> &nuc);

void write_EPS(std::vector<Nuclide> &in, inputs *draw, std::ofstream &out_file);

void write_SVG(std::vector<Nuclide> &in, inputs *draw, std::ofstream &out_file);

void get_exp(const float &in, std::vector<std::string> &out);

void iso_unit(const float &in, std::string &out);

void sec_human(const float &in, std::string &out);

void display_section(std::vector<Nuclide> &in, inputs *draw);

void setExtreme(std::string limit, inputs *draw);

void setNlimits(std::vector<Nuclide> &in, inputs *draw);

void setColours(std::vector<std::string> &kcol, std::vector<float> &n, inputs *draw);

void draw_nuclei(std::vector<Nuclide> &in, std::vector<std::string> &kcol, std::vector<float> &n, std::vector<bool> &k, inputs *draw, std::ofstream &out_file);
#endif

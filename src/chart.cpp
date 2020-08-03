#include "inch/chart.hpp"

#include <string_view>

#include "inch/chartColour.hpp"
#include "inch/dripline.hpp"
#include "inch/fileType.hpp"
#include "inch/grid.hpp"
#include "inch/key.hpp"
#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"
#include "inch/nuclide.hpp"
#include "inch/options.hpp"
#include "inch/prolog.hpp"
#include "inch/rProcess.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <cmath>
#include <filesystem>


// void Chart::writeSVG(const std::vector<Nuclide>& nuc, const Options& draw) const
//{
//  std::ofstream outFile(draw.outfile, std::ios::binary);
//
//  if (!outFile.is_open())
//    {
//      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
//      return;
//    }
//
//  const Prolog setup(size, draw.limits);
//  fmt::print(outFile, "{}", setup.SVGWriteProlog());
//
//  fmt::print(outFile, "<g transform=\"translate({0},{0}) scale({1},{1})\">\n", 0.5 * size, size);
//
//  drawNuclei(nuc, draw, outFile);
//
//  fmt::print(outFile,
//             "</g>\n"
//             "</svg>\n");
//
//  outFile.close();
//}
//
//
// void Chart::writeTIKZ(const std::vector<Nuclide>& nuc, const Options& draw) const
//{
//  std::ofstream outFile(draw.outfile, std::ios::binary);
//
//  if (!outFile.is_open())
//    {
//      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", draw.outfile);
//      return;
//    }
//
//  const Prolog setup(size, draw.limits);
//  fmt::print(outFile, "{}", setup.TIKZWriteProlog());
//
//  fmt::print(outFile,
//             "\\begin{{document}}\n"
//             "\\begin{{tikzpicture}}\n");
//
//  drawNuclei(nuc, draw, outFile);
//
//  fmt::print(outFile,
//             "\\end{{tikzpicture}}\n"
//             "\\end{{document}}\n");
//
//  outFile.close();
//}

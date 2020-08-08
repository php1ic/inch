#include "inch/tikz_chart.hpp"

#include "inch/nuclide.hpp"
#include "inch/options.hpp"

#include <fmt/ostream.h>

void TIKZChart::write(const std::vector<Nuclide>& massTable, const Partition& part) const
{
  std::ofstream outFile(options.outfile, std::ios::binary);

  if (!outFile.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to create the chart.\n", options.outfile);
      return;
    }

  fmt::print(outFile, "{}", prolog());

  fmt::print(outFile, "{}", setup());

  for (const auto& isotope : massTable)
    {
      if (isotope.show == 1 || isotope.show == 2)
        {
          fmt::print(outFile, "{}", isotope.writeAsTIKZ());
        }
    }

  fmt::print(outFile, "{}", teardown());
}


std::string TIKZChart::prolog() const
{
  return fmt::format("\\documentclass{{article}}\n"
                     "\\usepackage{{tikz}}\n"
                     "\\usepackage[T1]{{fontenc}}\n"
                     "\\usepackage[active,tightpage]{{preview}}\n"
                     "\\PreviewEnvironment{{tikzpicture}}\n"
                     "\n"
                     "%Setup command to draw box and text\n"
                     "\\newcommand{{\\nucleus}}[6][{}]{{\n"
                     "\\pgfmathsetmacro{{\\rc}}{{#1*sqrt(200)}}\n"
                     "\\filldraw[draw=black,thick,fill=#2,rounded corners=\\rc] (#3,#4) rectangle +(1,1)\n"
                     "+(0.5,0.75) node[anchor=mid,text=black] {{#5}}\n"
                     "+(0.5,0.27) node[anchor=mid,text=black] {{\\Large #6}};\n"
                     "}}\n",
                     curve);
}


std::string TIKZChart::setup() const
{
  return fmt::format("\\begin{{document}}\n"
                     "\\begin{{tikzpicture}}\n");
}


std::string TIKZChart::teardown() const
{
  return fmt::format("\\end{{tikzpicture}}\n"
                     "\\end{{document}}\n");
}

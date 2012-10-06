#include "functions.h"

void writeTIKZ(std::vector<Nuclide> &in, inputs *draw)
{
  std::ofstream out_file(draw->outfile.c_str());
  std::vector<Nuclide>::iterator nuc_it;

  if (out_file.is_open())
    {
      out_file << "\\documentclass{article}\n"
	       << "\\usepackage{tikz}\n"
	       << "\\usepackage[T1]{fontenc}\n"
	       << "\\usepackage[active,tightpage]{preview}\n"
	       << "\\PreviewEnvironment{tikzpicture}\n"
	       << "\\setlength\\PreviewBorder{" << 1.5*draw->size << "em}\n"
	       << "%Set scale factor\n"
	       << "\\newcommand{\\nsize}[0]{" << draw->size << "}\n"
	       << "%Set how round the corners are (0=square)\n"
	       << "\\newcommand{\\nround}[0]{" << draw->curve << "}\n"
	       << "%Construct the variable to apply the necessary rounding\n"
	       << "\\pgfmathsetmacro{\\rc}{\\nround*\\nsize*10*sqrt(2)}\n"
	       << "%Setup command to draw box and text\n"
	       << "\\newcommand{\\nucleus}[5]{\n"
	       << "\\filldraw[draw=black,thick,fill=#1,rounded corners=\\rc] (#2,#3) rectangle +(1,1)\n"
	       << "+(0.5,0.75) node[anchor=mid] {#4}\n"
	       << "+(0.5,0.27) node[anchor=mid] {\\Large #5};\n"
	       << "}\n"
	       << "\\begin{document}\n"
	       << "\\begin{tikzpicture}[scale=\\nsize, transform shape]\n" << std::endl;

      std::vector<float> n;
      n.assign(7,0);
      std::vector<std::string> kcol(11);
      std::vector<bool> k;
      k.assign(12,0);

      setColours(kcol,n,draw);

      drawNuclei(in,kcol,n,k,draw,out_file);

      out_file << "\\end{tikzpicture}\n" 
	       << "\\end{document}" << std::endl;

      out_file.close();
    }
    else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}

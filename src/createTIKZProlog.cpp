#include "functions.h"

void createTIKZProlog(const inputs *draw, std::ofstream &out_file)
{
  out_file << "\\documentclass{article}\n"
	   << "\\usepackage{tikz}\n"
	   << "\\usepackage[T1]{fontenc}\n"
	   << "\\usepackage[active,tightpage]{preview}\n"
	   << "\\PreviewEnvironment{tikzpicture}\n"
	   << "\\setlength\\PreviewBorder{" << 1.5*draw->size << "em}\n"
	   << "\n"
	   << "%Set scale factor\n"
	   << "\\newcommand{\\nsize}[0]{" << draw->size << "}\n"
	   << "\n"
	   << "%Set how round the corners are (0=square)\n"
	   << "\\newcommand{\\nround}[0]{" << draw->curve << "}\n"
	   << "\n"
	   << "%Construct the variable to apply the necessary rounding\n"
	   << "\\pgfmathsetmacro{\\rc}{\\nround*\\nsize*10*sqrt(2)}\n"
	   << "\n"
	   << "%Setup command to draw box and text\n"
	   << "\\newcommand{\\nucleus}[5]{\n"
	   << "\\filldraw[draw=black,thick,fill=#1,rounded corners=\\rc] (#2,#3) rectangle +(1,1)\n"
	   << "+(0.5,0.75) node[anchor=mid] {#4}\n"
	   << "+(0.5,0.27) node[anchor=mid] {\\Large #5};\n"
	   << "}\n" << std::endl;
}

#include "include/functions.h"

void createTIKZProlog(const inputs *draw,
		      std::ofstream &out_file
		      )
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
	   << "%Setup command to draw box and text\n"
	   << "\\newcommand{\\nucleus}[6][" << draw->curve << "]{\n"
	   << "\\pgfmathsetmacro{\\rc}{#1*\\nsize*10*sqrt(2)}\n"
	   << "\\filldraw[draw=black,thick,fill=#2,rounded corners=\\rc] (#3,#4) rectangle +(1,1)\n"
	   << "+(0.5,0.75) node[anchor=mid] {#5}\n"
	   << "+(0.5,0.27) node[anchor=mid] {\\Large #6};\n"
	   << "}\n" << std::endl;
}

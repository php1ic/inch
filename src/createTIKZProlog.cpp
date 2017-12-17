#include "functions.h"

void createTIKZProlog(const std::unique_ptr<inputs> &draw,
                      std::ofstream &outFile
                      )
{
  outFile << "\\documentclass{article}\n"
          << "\\usepackage{tikz}\n"
          << "\\usepackage[T1]{fontenc}\n"
          << "\\usepackage[active,tightpage]{preview}\n"
          << "\\PreviewEnvironment{tikzpicture}\n"
          << "\n"
          << "%Setup command to draw box and text\n"
          << R"(\newcommand{\nucleus}[6][)" << draw->curve << "]{\n"
          << "\\pgfmathsetmacro{\\rc}{#1*sqrt(200)}\n"
          << "\\filldraw[draw=black,thick,fill=#2,rounded corners=\\rc] (#3,#4) rectangle +(1,1)\n"
          << "+(0.5,0.75) node[anchor=mid,text=black] {#5}\n"
          << "+(0.5,0.27) node[anchor=mid,text=black] {\\Large #6};\n"
          << "}\n" << std::endl;
}

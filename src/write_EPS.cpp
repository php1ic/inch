#include "functions.h"

void write_EPS(std::vector<Nuclide> &in, inputs *draw)
{
  //-Get time/date from system
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  //-Get user from system
  uid_t uid=getuid();
  struct passwd *passwd;
  passwd = getpwuid(uid);

  //-Open the output file to write to
  std::ofstream out_file(draw->outfile.c_str());
  std::vector<Nuclide>::iterator nuc_it;

  if (out_file.is_open())
    {
      out_file << "%!PS-Adobe-3.0 EPSF-3.0\n"
	       << "%%Title: Nuclear Chart-";

           if (draw->choice == "a") out_file << "Error on mass-excess-";
      else if (draw->choice == "b") out_file << "Relative error on mass-excess-";
      else if (draw->choice == "c") out_file << "Major ground-state decay mode-";
      else if (draw->choice == "d") out_file << "Ground-state half-life-";
      else                          out_file << "First isomer energy-";

      out_file << "Z=[" << draw->Zmin << "(" << z_el(draw->Zmin) << ")," << draw->Zmax << "(" << z_el(draw->Zmax)
	       << ")]-N=[" << draw->Nmin << "," << draw->Nmax << "]\n"
	       << "%%BoundingBox: (atend)\n"
	       << "%%Creator: " << passwd->pw_name << " (" << passwd->pw_gecos << ")\n"
	       << "%%CreationDate: " << asctime(timeinfo)
	       << "%%DocumentFonts: Times-Roman Symbol\n"
	       << "%%Page: 1\n"
	       << "%%EndComments\n"
	       << "\nsystemdict /setdistillerparams known {\n"
	       << "<< /AutoFilterColorImages false /ColorImageFilter /FlateEncode >>\n"
	       << "setdistillerparams } if\n"
	       << "\n%%BeginProlog\n"
	       << "gsave 45 dict begin\n\n"
	       << "/bd {bind def} bind def\n"
	       << "/ld {load def} bd\n"
	       << "/u {" << draw->size << "} bd\n"
	       << "/curve {" << draw->curve << "} bd\n"
	       << "/ed {exch def} bd\n"
	       << "/TR {/Times-Roman exch selectfont} bd\n"
	       << "/S {/Symbol exch selectfont} bd\n\n"
	       << "/gs /gsave ld\n"
	       << "/gr /grestore ld\n"
	       << "/m /moveto ld\n"
	       << "/l /lineto ld\n"
	       << "/rl /rlineto ld\n"
	       << "/st /stroke ld\n"
	       << "/sh /show ld\n"
	       << "/rgb /setrgbcolor ld\n"
	       << "/sl /setlinewidth ld\n\n"
	       << "/c{\n"
	       << "dup stringwidth pop\n"
	       << "-2 div 0\n"
	       << "rmoveto sh\n"
	       << "} bd\n\n"
	       << "/A{\n"
	       << "gs\n"
	       << "x sw add y sw add translate\n"
	       << "0 sw m\n"
	       << "sw neg sw sw neg 0 r arct\n"
	       << "sw neg sw neg 0 sw neg r arct\n"
	       << "sw sw neg sw 0 r arct\n"
	       << "sw sw 0 sw r arct\n"
	       << "closepath clip\n"
	       << "gs\n"
	       << "sr sg sb rgb\n"
	       << "fill\n"
	       << "} bd\n"
	       << "\n"
	       << "/B{\n"
	       << "gr\n"
	       << "0.5 u div setlinewidth\n"
	       << "black rgb\n"
	       << "st\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/half{\n"
	       << "black rgb\n"
	       << "-0.5 0 m\n"
	       << "0 -0.5 rl\n"
	       << "1 0 rl\n"
	       << "0 0.5 rl\n"
	       << "fill\n"
	       << "} bd\n"
	       << "\n"
	       << "/wedge{\n"
	       << "black rgb\n"
	       << "-0.5 -0.5 m\n"
	       << "1 0 rl\n"
	       << "0 1 rl\n"
	       << "fill\n"
	       << "} bd\n"
	       << "\n"
	       << "/hbhalf{\n"
	       << "half\n"
	       << "} bd\n"
	       << "\n"
	       << "/hthalf{\n"
	       << "gs\n"
	       << "180 rotate\n"
	       << "half\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/vlhalf{\n"
	       << "gs\n"
	       << "270 rotate\n"
	       << "half\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/vrhalf{\n"
	       << "gs\n"
	       << "90 rotate\n"
	       << "half\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/brwedge{\n"
	       << "wedge\n"
	       << "} bd\n"
	       << "\n"
	       << "/trwedge{\n"
	       << "gs\n"
	       << "90 rotate\n"
	       << "wedge\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/tlwedge{\n"
	       << "gs\n"
	       << "180 rotate\n"
	       << "wedge\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/blwedge{\n"
	       << "gs\n"
	       << "270 rotate\n"
	       << "wedge\n"
	       << "gr\n"
	       << "} bd\n"
	       << "\n"
	       << "/w{\n"
	       << "/num ed\n"
	       << "/sym ed\n"
	       << "3 3 roll rgb\n"
	       << "/Times-Roman 0.6 selectfont\n"
	       << "x 0.5 add y 0.2 add m\n"
	       << "sym 4 string cvs c\n"
	       << "/Times-Roman 0.35 selectfont\n"
	       << "x 0.5 add y 0.65 add m\n"
	       << "num 4 string cvs c\n"
	       << "} bd\n"
	       << "\n"
	       << "/n{\n"
	       << "/r ed\n"
	       << "/y ed\n"
	       << "/x ed\n"
	       << "/sb ed\n"
	       << "/sg ed\n"
	       << "/sr ed\n"
	       << "/r r 2 div def\n"
	       << "/sw 1 2 div def\n"
	       << "count 1 eq\n"
	       << "{\n"
	       << "/h ed\n"
	       << "h 0 eq {A B} if\n"
	       << "h 1 eq {A hbhalf B} if\n"
	       << "h 2 eq {A hthalf B} if\n"
	       << "h 3 eq {A vlhalf B} if\n"
	       << "h 4 eq {A vrhalf B} if\n"
	       << "h 5 eq {A trwedge B} if\n"
	       << "h 6 eq {A tlwedge B} if\n"
	       << "h 7 eq {A brwedge B} if\n"
	       << "h 8 eq {A blwedge B} if\n"
	       << "h 8 gt {A B} if\n"
	       << "}\n"
	       << "{\n"
	       << "count 5 eq\n"
	       << "{A B w}\n"
	       << "{\n"
	       << "6 5 roll\n"
	       << "/h ed\n"
	       << "h 0 eq {A B w} if\n"
	       << "h 1 eq {A hthalf B w} if\n"
	       << "h 2 eq {A hbhalf B w} if\n"
	       << "h 3 eq {A vlhalf B w} if\n"
	       << "h 4 eq {A vrhalf B w} if\n"
	       << "h 5 eq {A trwedge B w} if\n"
	       << "h 6 eq {A tlwedge B w} if\n"
	       << "h 7 eq {A brwedge B w} if\n"
	       << "h 8 eq {A blwedge B w} if\n"
	       << "h 8 gt {A B w} if\n"
	       << "} ifelse\n"
	       << "} ifelse\n"
	       << "} bd\n\n"
	       << "/black     {0 0 0} bd\n"
	       << "/white     {1 1 1} bd\n"
	       << "/red       {1 0 0} bd\n"
	       << "/green     {0 1 0} bd\n"
	       << "/blue      {0 0 1} bd\n"
	       << "/yellow    {1 1 0} bd\n"
	       << "/magenta   {1 0 1} bd\n"
	       << "/cyan      {0 1 1} bd\n"
	       << "/orange    {1 0.6471 0} bd\n"
	       << "/darkgreen {0 0.3922 0} bd\n"
	       << "/navyblue  {0 0 0.5020} bd\n"
	       << "/purple    {0.6275 0.1255 0.9412} bd\n"
	       << "%%EndProlog\n"
	       << "\n%==The different ways to draw an isotope=======\n"
	       << "% \n"
	       << "%\n"
	       << "% Empty box\n"
	       << "% ---------\n"
	       << "% box_colour x y n\n"
	       << "%\n"
	       << "%\n"
	       << "% Box with the nuclei written inside\n"
	       << "% ----------------------------------\n"
	       << "% writing_colour (El) (num) box x y n\n"
	       << "%\n"
	       << "%\n"
	       << "% Empty box with half of it black\n"
	       << "% -------------------------------\n"
	       << "% x box_colour x y n\n"
	       << "%\n"
	       << "%\n"
	       << "% One black half with the nuclei written inside\n"
	       << "% ---------------------------------------------\n"
	       << "% x writing_colour (El) (num)  box_colour x y n\n"
	       << "%\n"
	       << "% Where x can take the values below to alter the\n"
	       << "% square as shown (x=0 creates a blank square).\n"
	       << "%\n"
	       << "% x    1      2      3      4\n"
	       << "%     ___    ___    ___    ___\n"
	       << "%    |###|  |   |  |#| |  | |#|\n"
	       << "%    |---|  |---|  |#| |  | |#|\n"
	       << "%    |___|  |###|  |#|_|  |_|#|\n"
	       << "%\n"
	       << "% x    5      6      7      8\n"
	       << "%     ___    ___    ___    ___\n"
	       << "%    |\\##|  |##/|  |  /|  |\\  |\n"
	       << "%    | \\#|  |#/ |  | /#|  |#\\ |\n"
	       << "%    |__\\|  |/__|  |/##|  |##\\|\n"
	       << "%\n"
	       << "%==============================================\n\n"
	       << "u dup scale\n"
	       << "0.5 0.5 translate" << std::endl;

      //-For positioning and alignment, draw a grid with spacings of 5 units.
      if (draw->grid)
	out_file << "\n%-Grid----------\n"
		 << "gs\n"
		 << "0.75 setgray\n"
		 << "2 TR\n"
		 << "0.1 u div sl\n"
		 << "\n5 5 " << draw->Nmax-draw->Nmin << "{\n"
		 << "/i ed\n"
		 << "i 1 m\n"
		 << "gs\n"
		 << "i 10 mod 0 eq {\n"
		 << "i 3 string cvs sh\n"
		 << "i " << draw->Zmax-draw->Zmin << " m i 3 string cvs sh\n"
		 << "} if\n"
		 << "gr\n"
		 << "0 " << draw->Zmax-draw->Zmin << " rl\n"
		 << "} for\n"
		 << "\n5 5 " << draw->Zmax-draw->Zmin << "{\n"
		 << "/i ed\n"
		 << "1 i m\n"
		 << "gs\n"
		 << "i 10 mod 0 eq {\n"
		 << "i 3 string cvs sh\n"
		 << "" << draw->Nmax-draw->Nmin-2 << " i m i 3 string cvs sh\n"
		 << "} if\n"
		 << "gr\n"
		 << draw->Nmax-draw->Nmin << " 0 rl\n"
		 << "} for\n"
		 << "\nst\n"
		 << "gr\n"
		 << "%---------------\n" << std::endl;

      //-Postscript doesn't support transparency, thus draw shaded
      //-area of the r-process before nuclei and the outline after.
      //-----------------------------
      //- r-process -- shaded path --
      //-----------------------------
      if (draw->r_process)
	draw_rprocess(draw,out_file,1);

      std::vector<float> n;
      n.assign(7,0);
      std::vector<std::string> kcol(11);
      std::vector<bool> k;
      k.assign(12,0);

      setColours(kcol,n,draw);

      draw_nuclei(in,kcol,n,k,draw,out_file);

      //-----------------
      //- Magic numbers -
      //-----------------
      if (draw->magic_numbers)
	draw_magicnumbers(draw,out_file);
      else
	std::cout << "\nNot drawing the magic numbers" << std::endl;

      //--------------
      //- Drip lines -
      //--------------
      if (draw->drip_lines > 0)
	draw_driplines(draw,out_file);
      else
	std::cout << "Drawing neither of the drip lines" << std::endl;

      //-----------------------------
      //- r-process -- path outline -
      //-----------------------------
      if (draw->r_process)
	draw_rprocess(draw,out_file,0);
      else
	std::cout << "Not drawing the r-process path" << std::endl;

      //-------
      //- Key -
      //-------
      float s=0;
      if (draw->key)
	draw_key(draw,out_file,s,kcol,k,n);
      else
	std::cout << "Not drawing the key" << std::endl;

      if (draw->section == "a" || (draw->Zmax-draw->Zmin) == 118)
	s=0;

      out_file << "end grestore\n\n"
	       << "%%Trailer\n"
	       << "%%BoundingBox: " << "0 0 " << (int) (draw->Nmax-draw->Nmin+2+14.5*s)*draw->size << " " << (draw->Zmax-draw->Zmin+2)*draw->size
	       << "\n%%EOF" << std::endl;

      out_file.close();
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->outfile << " to create the chart." << std::endl;
      exit(-1);
    }
}

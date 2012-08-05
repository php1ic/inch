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
  unsigned short i=0;

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
	{
	  //-May only want one of the drip lines
	  //------------------------------------
	  // drip_lines = 1 -> both
	  // drip_lines = 2 -> only p drip line
	  // drip_lines = 3 -> only n drip line
	  //------------------------------------

	  out_file << "\n%--------------\n"
		   << "%- Drip Lines -\n"
		   << "%--------------\n"
		   << "purple rgb\n"
		   << "1 u div sl" << std::endl;

	  if (draw->drip_lines != 3)
	    {
	      bool b;
	      short z_pdrip, n_pdrip;
	      float s_n;
	      std::string line;

	      out_file << "\n%--------------------\n"
		       << "%- Proton drip line -\n"
		       << "%--------------------\n";

	      //-File with proton drip line.
	      //-Format: N Z S(p)
	      draw->proton_drip.insert(0,draw->path);
	      std::ifstream p_drip(draw->proton_drip.c_str());

	      if (p_drip)
		{
		  std::cout << "Reading "
		       << draw->proton_drip.substr(draw->path.length(),draw->proton_drip.length()-draw->path.length())
		       << " and drawing the proton drip line";
		  b=false;

		  while(getline(p_drip,line))
		    {
		      if (line.at(0) != '#')
			{
			  std::istringstream in(line);

			  in >> n_pdrip >> z_pdrip >> s_n;

			  if (   z_pdrip >= draw->Zmin
			      && z_pdrip <= draw->Zmax
			      && n_pdrip >= draw->Nmin
			      && n_pdrip <= draw->Nmax)
			    {
			      out_file << std::setw(3) << n_pdrip-draw->Nmin << " "
				       << std::setw(3) << z_pdrip-draw->Zmin;

			      if (!b){out_file << " m\n"; b=true;}
			      else   out_file << " l\n";
			    }
			}
		    }
		  p_drip.close();
		}
	      else
		{
		  std::cout << "ERROR: " << draw->proton_drip
			    << " couldn't be opened to read the proton drip line" << std::endl;
		  exit(-1);
		}

	      std::cout << " - done\n";

	      out_file << "st" << std::endl;
	    }
	  else{std::cout << "Not drawing the proton drip line" << std::endl;}

	  if (draw->drip_lines != 2)
	    {
	      bool b;
	      short z_ndrip, n_ndrip;
	      float s_p;
	      std::string line;

	      out_file << "\n%---------------------\n"
		       << "%- Neutron drip line -\n"
		       << "%---------------------\n";

	      //-File with neutron drip line.
	      //-Format: N Z S(n)
	      draw->neutron_drip.insert(0,draw->path);
	      std::ifstream n_drip(draw->neutron_drip.c_str());

	      if (n_drip)
		{
		  std::cout << "Reading "
		       << draw->neutron_drip.substr(draw->path.length(),draw->neutron_drip.length()-draw->path.length())
		       << " and drawing the neutron drip line";
		  b=false;

		  while (getline(n_drip,line))
		    {
		      if (line.at(0) != '#')
			{
			  std::istringstream in(line);

			  in >> n_ndrip >> z_ndrip >> s_p;

			  if (   z_ndrip >= draw->Zmin
			      && z_ndrip <= draw->Zmax
			      && n_ndrip >= draw->Nmin
			      && n_ndrip <= draw->Nmax)
			    {
			      out_file << std::setw(3) << n_ndrip-draw->Nmin << " "
				       << std::setw(3) << z_ndrip-draw->Zmin;

			      if (!b){out_file << " m\n"; b=true;}
			      else   out_file << " l\n";
			    }
			}
		    }
		  n_drip.close();
		}
	      else
		{
		  std::cout << "ERROR: " << draw->neutron_drip
			    << " couldn't be opened to read the neutron drip line" << std::endl;
		  exit(-1);
		}

	      std::cout << " - done\n";

	      out_file << "st" << std::endl;
	    }
	  else{std::cout << "Not drawing the neutron drip line" << std::endl;}
	}
      else{std::cout << "Drawing neither of the drip lines" << std::endl;}

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
	{
	  std::cout << "Drawing the key ";
	  out_file << "\n%-------\n"
		   << "%- Key -\n"
		   << "%-------\n"
		   << "/rw {/sw 0 def} def\n"
		   << "/tw {dup stringwidth pop sw add /sw exch def} def\n"
		   << "/kx 0 def\n"
		   << "/tx {sw kx gt{/kx sw def}if} def\n\n"
		   << "%-lower left corner of the key-\n";

	  if (draw->choice == "a")
	    {
	      std::string kword[8];
	      std::ostringstream low,high;

	      low<<n[0];

	      kword[0] = "1 TR (Stable \\() tw sh\n1 S (d) tw sh\n1 TR (m < " + low.str() + " keV\\)) tw sh tx\n\n";
	      kword[1] = "1 TR (Stable \\() tw sh\n1 S (d) tw sh\n1 TR (m > " + low.str() + " keV\\)) tw sh tx\n";
	      kword[2] = "1 S (d) tw sh\n1 TR (m < " + low.str() + " keV) tw sh tx\n";

	      high<<n[1];

	      kword[3] = "1 TR (  " + low.str() + " keV < ) tw sh\n1 S (d) tw sh\n1 TR (m < " + high.str() + " keV) tw sh tx\n";

	      for (i=1;i<4;i++)
		{
		  low.str("");
		  low<<n[i];
		  high.str("");
		  high<<n[i+1];
		  kword[i+3] = "1 TR (" + low.str() + " keV < ) tw sh\n1 S (d) tw sh\n1 TR (m < " + high.str() + " keV) tw sh tx\n";
		}

	      kword[7] = "1 S (d) tw sh\n1 TR (m > " + high.str() + " keV) tw sh tx\n";

	      float y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    y+=1.5;
		}

	      if ((draw->Zmax-draw->Zmin)/y>3.0)
		s=3;
	      else
		s=(draw->Zmax-draw->Zmin)/y;

	      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
		{
		  s=3;
		  out_file << "15 75 translate\n";
		}
	      else
		out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

	      out_file << s << " dup scale\n" << std::endl;
	      y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    {
		      out_file << "0 " << kcol[11-i] << " 0.5 " << y << " curve n\n"
			       << "2.5 " << y+0.2 << " m rw\n"
			       << kword[11-i];
		      y+=1.5;
		    }
		}

	      out_file << "\n0 0 m\n"
		       << "kx 3 add 0 rl\n"
		       << "0 " << y << " rl\n"
		       << "kx 3 add neg 0 rl\n"
		       << "closepath\n"
		       << "st\n" << std::endl;
	    }
	  else if (draw->choice == "b")
	    {
	      std::vector<std::string> low(2), high(2), kword(6);

	      get_exp(n[0],low);
	      kword[0] = "1 S (d) tw sh\n1 TR (m/m < ) tw sh\n" + low[0] + " -" + low[1] + " e tx\n";

	      get_exp(n[1],high);
	      kword[1] = low[0] + " -" + low[1] + " e f " + high[0] + " -" + high[1] + " e tx\n";

	      for (i=1;i<4;i++)
		{
		  get_exp(n[i],low);
		  get_exp(n[i+1],high);
		  kword[i+1] = low[0] + " -" + low[1] + " e f " + high[0] + " -" + high[1] + " e tx\n";
		}

	      kword[5] = "1 S (d) tw sh\n1 TR (m/m > ) tw sh\n" + high[0] + " -" + high[1] + " e tx\n";

	      float y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    y+=1.5;
		}

	      if ((draw->Zmax-draw->Zmin)/y>3.0)
		s=3;
	      else
		s=(draw->Zmax-draw->Zmin)/y;

	      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
		{
		  s=3;
		  out_file << "12 81 translate\n";
		}
	      else
		out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

	      y=0.5;
	      out_file << s << " dup scale\n"
		       << "\n/e{\n"
		       << "/e1 ed\n"
		       << "/e2 ed\n"
		       << "1 TR e2 2 string cvs tw sh\n"
		       << "0.75 TR (x) tw sh\n"
		       << "1 TR (10) tw sh\n"
		       << "gs\n"
		       << "0.75 TR\n"
		       << "0 0.4 rmoveto e1 2 string cvs tw sh\n"
		       << "gr\n"
		       << "} def\n\n"
		       << "/f{\n"
		       << "1 TR (   < ) tw sh\n"
		       << "1 S (d) tw sh\n"
		       << "1 TR (m/m < ) tw sh\n"
		       << "} def\n" << std::endl;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    {
		      out_file << "0 " << kcol[11-i] << " 0.5 " << y << " curve n\n"
			       << "2.5 " << y+0.2 << " m rw\n"
			       << kword[11-i];
		      y+=1.5;
		    }
		}

	      out_file << "\n0 0 m\n"
		       << "kx 3 add 0 rl\n"
		       << "0 " << y << " rl\n"
		       << "kx 3 add neg 0 rl\n"
		       << "closepath\n"
		       << "st\n" << std::endl;
	    }
	  else if (draw->choice == "c")
	    {
	      std::vector<std::string> kword(11);

	      kword[0] 	= "1 TR (Stable) tw sh tx\n";
	      kword[1] 	= "1 TR (Alpha) tw sh tx\n";
	      kword[2] 	= "1 S (b) tw sh\n0.5 TR 0 0.55 rmoveto (+) tw sh tx\n";
	      kword[3] 	= "1 S (b) tw sh\n0.75 TR 0 0.55 rmoveto (-) tw sh tx\n";
	      kword[4] 	= "1 TR (Spontaneous Fission)tw sh tx\n";
	      kword[5] 	= "1 TR (n decay) tw sh tx\n";
	      kword[6] 	= "1 TR (2n decay) tw sh tx\n";
	      kword[7] 	= "1 TR (p decay) tw sh tx\n";
	      kword[8] 	= "1 TR (2p decay) tw sh tx\n";
	      kword[9] 	= "1 TR (Unknown) tw sh tx\n";
	      kword[10] = "1 TR (Electron Capture) tw sh tx\n";

	      float y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    y+=1.5;
		}

	      if ((draw->Zmax-draw->Zmin)/y>3.0)
		s=3;
	      else
		s=(draw->Zmax-draw->Zmin)/y;

	      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
		{
		  s=3;
		  out_file << "9 60 translate\n";
		}
	      else
		out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

	      out_file << s << " dup scale\n" << std::endl;
	      y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    {
		      out_file << "0 " << kcol[11-i] << " 0.5 " << y << " curve n\n"
			       << "2.5 " << y+0.2 << " m rw\n"
			       << kword[11-i];
		      y+=1.5;
		    }
		}

	      out_file << "\n0 0 m\n"
		       << "kx 3 add 0 rl\n"
		       << "0 " << y << " rl\n"
		       << "kx 3 add neg 0 rl\n"
		       << "closepath\n"
		       << "st\n" << std::endl;
	    }
	  else if (draw->choice == "d")
	    {
	      std::string kword[8];
	      std::string low,high;

	      sec_human(n[0],low);
	      kword[0] = "t 1 TR (     < " + low + ") tw sh tx\n";

	      sec_human(n[1],high);
	      kword[1] = "1 TR (" + low + " < ) tw sh t\n(     < " + high + ") tw sh tx\n";

	      for (i=1;i<7;i++)
		{
		  low=high;
		  sec_human(n[i],high);
		  kword[i] = "1 TR (" + low + " < ) tw sh t\n(     < " + high + ") tw sh tx\n";
		}

	      kword[7] 	= "t 1 TR (     > " + high + ") tw sh tx\n";

	      float y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    y+=1.5;
		}

	      if ((draw->Zmax-draw->Zmin)/y>3.0)
		s=3;
	      else
		s=(draw->Zmax-draw->Zmin)/y;

	      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
		{
		  s=3;
		  out_file << "9 75 translate\n";
		}
	      else
		out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

	      y=0.5;
	      out_file << s << " dup scale\n"
		       << "\n/t{gs\n"
		       << "1 S (t) sh\n"
		       << "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
		       << "gr} def\n" << std::endl;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    {
		      out_file << "0 " << kcol[11-i] << " 0.5 " << y << " curve n\n"
			       << "2.5 " << y+0.2 << " m rw\n"
			       << kword[11-i];
		      y+=1.5;
		    }
		}

	      out_file << "\n0 0 m\n"
		       << "kx 3 add 0 rl\n"
		       << "0 " << y << " rl\n"
		       << "kx 3 add neg 0 rl\n"
		       << "closepath\n"
		       << "st\n" << std::endl;
	    }
	  else if (draw->choice == "e")
	    {
	      std::string kword[7];
	      std::string low,high;

	      iso_unit(n[0],low);
	      kword[0] = "1 TR (E < " + low + ") tw sh tx\n";

	      iso_unit(n[1],high);
	      kword[1] = "1 TR (" + low + " < E < " + high + ") tw sh tx\n";

	      low=high;
	      iso_unit(n[2],high);
	      kword[2] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

	      low=high;
	      iso_unit(n[3],high);
	      kword[3] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

	      low=high;
	      iso_unit(n[4],high);
	      kword[4] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

	      kword[5] = "1 TR (E > " + high + ") tw sh tx\n";
	      kword[6] = "1 TR (No known isomer) tw sh tx\n";

	      float y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    y+=1.5;
		}

	      if ((draw->Zmax-draw->Zmin)/y>3.0)
		s=3;
	      else
		s=(draw->Zmax-draw->Zmin)/y;

	      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
		{
		  s=3;
		  out_file << "15 75 translate\n";
		}
	      else
		out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

	      out_file << s << " dup scale\n" << std::endl;
	      y=0.5;
	      for (i=0;i<12;i++)
		{
		  if (k[11-i]==1)
		    {
		      out_file << "0 " << kcol[11-i] << " 0.5 " << y << " curve n\n"
			       << "2.5 " << y+0.2 << " m rw\n"
			       << kword[11-i];
		      y+=1.5;
		    }
		}

	      out_file << "\n0 0 m\n"
		       << "kx 3 add 0 rl\n"
		       << "0 " << y << " rl\n"
		       << "kx 3 add neg 0 rl\n"
		       << "closepath\n"
		       << "st\n" << std::endl;
	    }

	  std::cout << "- done" << std::endl;
	}
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

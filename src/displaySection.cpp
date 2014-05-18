#include "include/functions.h"

void displaySection(std::vector<Nuclide> &in,
		    inputs *draw
		    )
{
  std::vector<Nuclide>::iterator nuc_it;
  int
    stbl_Zmin=MAX_N, stbl_Zmax=MIN_N,
    Nmin_Zmin=MAX_N, Nmin_Zmax=MAX_N,
    Nmax_Zmin=MIN_N, Nmax_Zmax=MIN_N;

  std::cout << "\n---------------------------\n"
	    << "Draw a) The entire chart\n"
	    << "     b) A section\n";
  do
    {
      std::cout << "[a,b]: ";
      std::cin  >> draw->section;

      if (draw->section == "a")
	{
	  draw->Zmin = MIN_Z;
	  draw->Nmin = MIN_N;
	  draw->Zmax = MAX_Z;
	  draw->Nmax = MAX_N;
	}
      else if (draw->section == "b")
	{
	  std::cout << "---------------------------\n"
		    << "Enter range of Z, by symbol [n,Ei] or number [0," << MAX_Z << "]\n";

	  setExtreme("Zmin",draw);

	  setExtreme("Zmax",draw);

	  std::cout << "---------------------------\n"
		    << "Draw a) All required N\n"
		    << "     b) A section\n";
	  do
	    {
	      std::cout << "[a,b]: ";
	      std::cin  >> draw->required;

	      if (draw->required == "a")
		{
		  setNeutronLimits(in,draw);
		}
	      else if (draw->required == "b")
		{
		  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
		    {
		      //Set N range for Zmin
		      if (nuc_it->Z == draw->Zmin)
			{
			  if (nuc_it->N < Nmin_Zmin)
			    Nmin_Zmin = nuc_it->N;
			  else if (nuc_it->N > Nmax_Zmin)
			    Nmax_Zmin = nuc_it->N;
			}
		      //Set N range for Zmax
		      else if (nuc_it->Z == draw->Zmax)
			{
			  if (nuc_it->N < Nmin_Zmax)
			    Nmin_Zmax = nuc_it->N;
			  else if (nuc_it->N > Nmax_Zmax)
			    Nmax_Zmax = nuc_it->N;
			}
		    }

		  //Set high/low stable N for Zmax/Zmin
		  for (nuc_it=in.begin(); nuc_it!=in.end(); ++nuc_it)
		    {
		      if (    nuc_it->N >= Nmin_Zmin
			   && nuc_it->N <= Nmax_Zmax
			   && nuc_it->decay == "stable"
			  )
			{
			  if (nuc_it->Z == draw->Zmin && nuc_it->N < stbl_Zmin)
			    stbl_Zmin = nuc_it->N;
			  if (nuc_it->Z == draw->Zmax && nuc_it->N > stbl_Zmax)
			    stbl_Zmax = nuc_it->N;
			}
		    }

		  std::cout << "---------------------------\n"
			    << "Enter range of N [0," << MAX_N << "]\n"
			    << convertZToSymbol(draw->Zmin) << "(" << draw->Zmin << ") has N from "
			    << Nmin_Zmin << " to " << Nmax_Zmin;

		  if (draw->Zmin > 83 || draw->Zmin == 43 || draw->Zmin == 0)
		    std::cout << " with no stable isotope\n";
		  else
		    std::cout << " and the lightest stable isotope has N=" << stbl_Zmin << "\n";

		  setExtreme("Nmin",draw);

		  std::cout << convertZToSymbol(draw->Zmax) << "(" << draw->Zmax << ") has N from "
			    << Nmin_Zmax << " to " << Nmax_Zmax;

		  if (draw->Zmax > 83 || draw->Zmax == 43 || draw->Zmax == 0)
		    std::cout << " with no stable isotope\n";
		  else
		    std::cout << " and the heaviest stable isotope has N=" << stbl_Zmax << "\n";

		  setExtreme("Nmax",draw);
		}
	      else
		std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
	    }
	  while (draw->required != "a" && draw->required != "b");
	}
      else
	std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
    }
  while (draw->section != "a" && draw->section != "b");

  std::cout << "---------------------------\n"
	    << "Display which nuclei?\n"
	    << "a) Experimentally measured only\n"
	    << "b) Theoretical/Extrapolated values only\n"
	    << "c) Both\n";
  do
    {
      std::cout << "Which: ";
      std::cin  >> draw->type;

      if (draw->type != "a" && draw->type != "b" && draw->type != "c")
	std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
    }
  while (draw->type != "a" && draw->type != "b" && draw->type != "c");

  if      (draw->type == "a") draw->experimental = 1;
  else if (draw->type == "b") draw->experimental = 0;
  else                        draw->experimental = 2;

  std::cout << "---------------------------\n"
	    << "Colour by which property?\n"
	    << "a) Error on Mass-Excess\n"
	    << "b) Relative Error on Mass-Excess (dm/m)\n";

  if (!draw->AME)
    {
      std::cout << "c) Major Ground-State Decay Mode\n"
		<< "d) Ground-State Half-Life\n";
      if (draw->type!="b")
	std::cout << "e) First Isomer Energy\n";
    }

  bool validChoice=false;
  while (!validChoice)
    {
      std::cout << "Choice: ";
      std::cin  >> draw->choice;

      if (draw->AME)
	{
	  if (draw->choice != "a" && draw->choice != "b")
	    std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
	  else if (draw->choice == "a" || draw->choice == "b")
	    validChoice=true;
	}
      else
	{
	  if (  (draw->type == "a" || draw->type== "c")
	      && draw->choice != "a"
	      && draw->choice != "b"
	      && draw->choice != "c"
	      && draw->choice != "d"
	      && draw->choice != "e"
	      )
	    std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
	  else if (   draw->type == "b"
		   && draw->choice != "a"
		   && draw->choice != "b"
		   && draw->choice != "c"
		   && draw->choice != "d"
		   )
	    std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
	  else
	    validChoice=true;
	}
    }
  /*
  std::cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"
	    << "Z = " << draw->Zmin << " -> " << draw->Zmax << "\n"
	    << "N = " << draw->Nmin << " -> " << draw->Nmax << "\n"
	    << "===========================\n"
	    << draw->Zmin << " : " << Nmin_Zmin << " " << stbl_Zmin << " " << Nmax_Zmin << "\n"
	    << draw->Zmax << " : " << Nmin_Zmax << " " << stbl_Zmax << " " << Nmax_Zmax << "\n"
	    << "===========================\n"
	    << draw->Zmin << " : " << draw->Zmin+draw->Nmin << "  " << draw->Zmin+Nmax_Zmin << "\n"
	    << draw->Zmax << " : " << draw->Zmax+draw->Nmin << "  " << draw->Zmax+Nmax_Zmax << "\n"
	    << "___________________________\n" << std::endl;
  */
}

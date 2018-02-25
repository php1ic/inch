#include "massTable.hpp"

MassTable::MassTable(std::string path,
                     const int year):
  table_year(year),
  data_path(std::move(path))
{
}

MassTable::MassTable(std::string path):
  data_path(std::move(path))
{
}

void MassTable::populateInternalMassTable()
{
  setFilePaths(table_year);

  //-Read mass table
  if ( !readNUBASE(mass_table_NUBASE) )
    {
      std::cout << "Nuclear data has not been read, exiting..." << std::endl;
      exit(-1);
    }

  if ( !readAME(mass_table_AME) )
    {
      std::cout << "Values from AME were not read." << std::endl;
    }

  //-Read user defined nuclei
  if ( ! user_isotopes.empty() )
    {
      if ( !readOWN(user_isotopes) )
        {
          std::cout << "User defined nuclei have not been read." << std::endl;
        }
    }
  else
    {
      std::cout << "Not drawing any user selected nuclei" << std::endl;
    }
}


bool MassTable::readAME(const std::string &ameTable)
{
  std::cout << "Reading " << ameTable.substr(ameTable.find_last_of('/')+1)
            << " for updated mass excess values [--";

  std::ifstream file(ameTable.c_str());

  if ( !file )
    {
      std::cout << "\n"
                << "***ERROR***: "
                << ameTable << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  int i=0;
  std::string line;

  while ( getline(file,line) )
    {
      // Skip the header of the file
      if ( i < AME_HEADER_LENGTH )
        {
          ++i;
          continue;
        }

      int A=0;
      int Z=0;

      // Will use mass excess for criteria, the last digit is char 52 so if
      // there is a '#' but it's after this we will still say experimental
      size_t measured = line.find_first_of('#');
      int exp = ( measured > AME_EXPERIMENTAL_MARKER )
        ? 1
        : 0;

      if ( measured != std::string::npos )
        {
          replace(line.begin(),line.end(),'#',' ');
        }

      extractValue(line,AME_START_A,AME_END_A,A);

      extractValue(line,AME_START_Z,AME_END_Z,Z);

      for ( auto &it : theTable )
        {
          if (   it.st == 0
              && it.A  == A
              && it.Z  == Z
              )
            {
              it.setAMEMassExcess(line);

              it.setAMEMassExcessError(line);

              it.setExperimental(exp);

              break;
            }
        }
    }

  file.close();

  std::cout << "--] done" << std::endl;
  return true;
}


bool MassTable::readNUBASE(const std::string &nubaseTable)
{
  std::cout << "Reading " << nubaseTable.substr(nubaseTable.find_last_of('/')+1)
            << " for nuclear values <--";

  std::ifstream file(nubaseTable.c_str());

  if ( !file )
    {
      std::cout << "\n"
                << "***ERROR***: " << nubaseTable
                << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::vector<bool> pnSide(MAX_Z+1, false);
  std::string line;

  while ( getline(file,line) )
    {
      if ( line.find("non-exist") != std::string::npos )
        {
          continue;
        }

      Nuclide isotope(line);

      isotope.setSpinParity();

      isotope.setExperimental();

      isotope.setA();

      isotope.setZ();

      isotope.setN();

      isotope.setState();

      isotope.setNubaseMassExcess();

      isotope.setNubaseMassExcessError();

      if ( isotope.st == 0 && isotope.A > 1 )
        {
          isotope.setSeparationEnergies(theTable);
        }

      isotope.setIsomerEnergy();

      isotope.setHalfLife();

      isotope.setDecayMode(pnSide,table_year);

      isotope.setNeutronOrProtonRich(pnSide);

      theTable.emplace_back(isotope);
    }

  file.close();

  std::cout << "--> done" << std::endl;
  return true;
}


bool MassTable::readOWN(const std::string &ownTable)
{
  std::cout << "Reading " << ownTable.substr(ownTable.find_last_of('/')+1)
            << " for user selected nuclei (--";

  std::ifstream inFile(ownTable.c_str());

  if ( !inFile )
    {
      std::cout << "\n"
                << "***ERROR***: " <<  ownTable
                << " couldn't be opened.\n" << std::endl;
      return false;
    }

  std::string line;

  while ( getline(inFile,line) )
    {
      if ( line.empty() || line.at(0) == '#' )
        {
          continue;
        }

      int N=0;
      int Z=0;
      int st=0;

      std::istringstream ownData(line);
      ownData >> N >> Z >> st;

      for ( auto &it : theTable )
        {
          if(   it.N  == N
             && it.Z  == Z
             && it.st == st
             )
            {
              it.setOwn(true);
              break;
            }
        }
    }

  inFile.close();

  std::cout << "--) done" << std::endl;

  return true;
}


void MassTable::setFilePaths(const int tableYear)
{
  switch ( tableYear )
    {
    default:
    case 3:
      mass_table_NUBASE = data_path + "nubtab03.asc";
      mass_table_AME    = data_path + "mass.mas03";
      break;
    case 12:
      mass_table_NUBASE = data_path + "nubase.mas12";
      mass_table_AME    = data_path + "mass.mas12";
      break;
    case 16:
      mass_table_NUBASE = data_path + "nubase2016.txt";
      mass_table_AME    = data_path + "mass16.txt";
      break;
    }
}

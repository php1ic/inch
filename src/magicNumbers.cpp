#include "magicNumbers.hpp"

void MagicNumbers::constructMap() const
{
  for (size_t i = 0; i < numbers.size(); ++i)
    {
      coords.emplace(std::make_pair(numbers.at(i), values.at(i)));
    }
}


void MagicNumbers::EPSWriteProtonNumber(std::ofstream& outFile, const int number) const
{
  const Number position = coords.at(number);

  const int min = (position.min_n - Nmin < 0) ? min_val : position.min_n - Nmin;

  const int max = ((position.max_n - Nmin) > (Nmax - Nmin + max_val)) ? Nmax - Nmin + max_val : position.max_n - Nmin;

  outFile << "%Z=" << number << '\n'
          << min << " " << number - Zmin + 1 << " m " << max - min << " 0 rl\n"
          << min << " " << number - Zmin << " m " << max - min << " 0 rl st" << std::endl;
}


void MagicNumbers::EPSWriteNeutronNumber(std::ofstream& outFile, const int number) const
{
  const Number position = coords.at(number);

  const int min = (position.min_z - Zmin < 0) ? min_val : position.min_z - Zmin;

  const int max = (position.max_z - Zmin > (Zmax - Zmin + max_val)) ? Zmax - Zmin + max_val : position.max_z - Zmin;

  outFile << "%N=" << number << '\n'
          << number - Nmin + 1 << " " << min << " m 0 " << max - min << " rl\n"
          << number - Nmin << " " << min << " m 0 " << max - min << " rl st" << std::endl;
}

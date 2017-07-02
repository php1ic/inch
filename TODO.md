This file contains known bugs and a list of features and additions that may make the program better and/or more usable.

Additions are encouraged, you can either contact the author or implement the change and send a pull request via github - https://github.com/php1ic/inch

## Known Bugs

- The function [convertFloatToExponent()](src/convertFloatToExponent.cpp) assumes that the value of the float will be <1 as was written with dm/m in mind.

- The format of the data file containing user defined nuclei is not check/validated.

- The options to the questions asked at runtime vary depending on certain options, e.g. theoretical only values removes option to show isomers, or using AME limits to mass excess and dm/m. When checking the input file, these factors are not taken into effect.

- The script [random_chart.sh](scripts/random_chart.sh) will fail if the user sets the the minimum allowed value of Z (`low_max_z`) to be the same as `max_z`. Need to catch this scenario, prevent it from happening or both.

## Things that need to be looked at (Not quite bugs)

- The population of the member [Nuclide::decay](src/nuclide.cpp#L437) needs to be looked at in relation to isotopes that have many different values/possibilities

- The population of the member [Nuclide::jpi](sr/nuclide.cpp#L87) needs to be looked at in relation to isotopes that have many different values/possibilities

- The fonts used in the eps version of the charts are not easily available with the version of texlive that comes with fedora. Thus, if used in a latex document and the user does dvi->ps (using dvips) characters can get 'lost'. This is most obvious in the key. Not sure if this is an issue with INCH or just an artefact of the licensing rules that fedora follow.

## Possible improvements/alterations (in no particular order)

- Use useful exit values if we quit out before completion, e.g. 10=no mass table 11=bad input file, etc...

- The default EPS file is automatically overwritten if no output file is given. Either make it so the user needs to ALWAYS specify a file name or add the option not to overwrite it.

- Add a comment about how the drip lines were calculated.

- It is harder to select by N than by A, make selection by A possible.

- Add the option for spin and parity colouring. Not sure what the choices would be beyond odd/even.

- Add the option for colouring by higher isomeric states. The mass table seems fairly selective on the non-ground state states included so this may not be very good. Not sure how complete the first isomer list actually is.

- If the nuclei also decays via another branch with a non negligible fraction (>5%) alter how it is displayed (may need to be quantitative, could get messy).

- Write isotopic abundances in the stable isotopes, again could get messy.

- Have option to print out the details for a specific isotope. The information is already collected, this could be an option at the start. Either draw a chart or get info for an isotope.

- Add comments about how to alter the text in the box. Perhaps make a guide, explaining both the basics of the postscript code and how to make changes once the chart is created.

- If an output name is given for the created chart file, use this base for the option file that is also created.

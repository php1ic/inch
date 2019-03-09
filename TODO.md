This file contains known bugs and a list of features and additions that may make the program better and/or more usable.

Additions are encouraged, you can either contact the author or implement the change and send a pull request via [github](https://github.com/php1ic/inch) or [gitlab](https://gitlab.com/php1ic/inch)

## Known Bugs

- The format of the data file containing user defined nuclei is not check/validated.

- The options to the questions asked at runtime vary depending on certain options, e.g. theoretical only values removes option to show isomers, or using AME limits to mass excess and dm/m.
When checking the input file, these factors are not taken into effect.

- The values starting at column 73 is being totally ignored. Looks like it's the error on the half-life, not sure why it was missed.

- The 2012 and 2016 nubase files added isospin, but in a way that can overlap with the spin parity values and the error on half-life.
Two examples are:

```
007 0048   7Bei    26750       30     10980      30     RQ                     3/2-    T=3/2 03               p ?;3He ?;A ?
007 0050   7B      27677       25                            570     ys 140    (3/2-)        03 11Ch32t  1967 p=100
008 0020   8He     31609.68     0.09                         119.1   ms 1.2    0+            05          1965 B-=100;B-n=16 1;B-t=0.9 1
008 0030   8Li     20945.80     0.05                         839.40  ms 0.36   2+            05 10Fl01t  1935 B-=100;B-A=100
008 0038   8Lii    31768        5     10822       5     RQ                     0+      T=2   05
008 0040   8Be      4941.67     0.04                          81.9   as 3.7    0+            05          1932 A=100
008 0048W  8Bei    21568        3     16626       3                            2+  frg T=1      04Ti06e  2004 A~100
```
and
```
011 0050   11B      8667.9      0.4                          stbl              3/2-          12          1920 IS=80.1 7
011 0058   11Bxi   21228        9     12560       9     RQ              T=3/2  1/2+,(3/2+)   12          1963
011 0060   11C     10650.3      0.9                           20.364  m 0.014  3/2-          12          1934 B+=100
011 0068   11Cxi   22810       40     12160      40     RQ                     1/2+    T=3/2 12 71Wa21d  1971 p=?
```
There are isospin values for 195/5511 in 2012 and 205/5625 in 2016.
Currently I'm inclined to simply remove them from the line rather than extract all of the necessary situations in order to correctly parse.

- If the user gives the (N,Z) of a currently undiscovered isotope to be marked as 'one of theirs', it will not be drawn.
An instance of [Nuclide](include/nuclide.cpp) is created for isotopes in the mass table and [MassTable::readOWN](src/massTable.cpp#L185) just reads the given file, toggling [Nuclide::own](include/nuclide.hpp#L72) for the appropriate isotope.
Will need to either add a new instance to [MassTable::theTable](include/massTable.hpp#L51), or create a new container, for such cases.

## Things that need to be looked at (Not quite bugs)

- [#7](https://github.com/php1ic/inch/issues/7) - Add tests for the copious amounts of IO.

- [#4](https://github.com/php1ic/inch/issues/4) - Make the struct [Partition::section](src/partition.hpp#L35) a template on Partition::section::value.
This should allow the removal of a lot of code bloat when setting isotope attributes.

- In the eps file, we set up (set linewidth, colour etc) to draw the r-process, drip lines and magic numbers before checking if they are actually drawn.
Thus it's possible that we set-up then tidy-up without actually drawing anything.
Make the necessary changes so this doesn't happen.

- There are a lot of magic numbers scattered throughout the code, mainly due to the formatting of the data files, e.g position of the mass-excess value in the NUBASE, or the header size for AME.
Look into either refactoring them **ALL** into a single header (or enum?) or individually per file/mass database.

- The population of the member [Nuclide::decay](src/nuclide.cpp#L437) needs to be looked at in relation to isotopes that have many different values/possibilities

- The population of the member [Nuclide::jpi](src/nuclide.cpp#L87) needs to be looked at in relation to isotopes that have many different values/possibilities

## Possible improvements/alterations (in no particular order)

- Use useful exit values if we quit out before completion, e.g. 10=no mass table 11=bad input file, etc...

- The default EPS file is automatically overwritten if no output file is given.
Either make it so the user needs to ALWAYS specify a file name or add the option not to overwrite it.

- Add a comment about how the drip lines were calculated.

- It is harder to select by N than by A, make selection by A possible.

- Add the option for spin and parity colouring.
Not sure what the choices would be beyond odd/even.

- Add the option for colouring by higher isomeric states.
The mass table seems fairly selective on the non-ground state states included so this may not be very good.
Not sure how complete the first isomer list actually is.

- If the nuclei also decays via another branch with a non negligible fraction (>5%) alter how it is displayed (may need to be quantitative, could get messy).

- Write isotopic abundances in the stable isotopes, again could get messy.

- Have option to print out the details for a specific isotope.
The information is already collected, this could be an option at the start.
Either draw a chart or get info for an isotope.

- Add comments about how to alter the text in the box.
Perhaps make a guide, explaining both the basics of the postscript code and how to make changes once the chart is created.

- Colour boundaries/partitions are hard coded.
This should be automated.

# INCH - the Interactive Nuclear CHart

INCH is a C++ project aimed at providing a simple method of creating high quality (preferably vector graphic) images of a bespoke region of the nuclear, sometimes referred to as Segre, chart.

No guarantee is supplied with regards to the accuracy of the data presented. Estimated values are included, please always refer to the original sources. All data should, however, be accurate.

Suggestions/improvements are encouraged (bugs less so), you can either contact the author or implement the change and send a pull request via github - https://github.com/php1ic/inch

The code reads data from the following published papers:
- [The Atomic Mass Evaluation](http://www.sciencedirect.com/science/article/pii/S0375947403018086) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.002)
- [The NUBASE evaluation of nuclear and decay properties](http://www.sciencedirect.com/science/article/pii/S0375947403018074) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.001)

The data contained within both are freely available as ASCII files from the [AMDC](https://www-nds.iaea.org/amdc/) website.


## Building

The project can be built using either [cmake](https://cmake.org/) or [GNU Make](https://www.gnu.org/software/make/) depending on your preference. Instructions on building can be found in the file [INSTALL.md](INSTALL.md).

The following files within *./data_files/* should **NOT** be altered:

```
FRLDM_ME.tbl
mass.mas114
nubtab03.asc
```

Any other files in *./data_files/* are used for extra functionality and in some cases are recreated if not present. As always, if you aren't sure, don't change it and if you do, backup!


## Versioning
There is an attempt to follow [SemVer](http://semver.org/) for versioning numbers. For the currently available versions, see the [tags on this repository](https://github.com/php1ic/inch/tags).

As it currently stands, the available outputs are:

- Creation of the [Encapsulated Postscript (*.eps)](https://en.wikipedia.org/wiki/Encapsulated_PostScript) file
  * with all functionality is complete bar minor tweaks/bug fixes.

- Creation of the [Scalable Vector Graphics (*.svg)](https://www.w3.org/TR/SVG/) file
  * is at the basic stage, a general function exists to position, create
  and colour the box. The addition of nuclei text is in progress.

- Creation of the [TikZ](https://www.ctan.org/pkg/pgf) file
  * is at the basic stage, a general function exists to position, create
  and colour the box whilst adding text.


## License
The code may be freely copied, distributed and altered under the terms of the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html) license. Please read the file [COPYING](COPYING), which should be located at the root of the project, for full details.

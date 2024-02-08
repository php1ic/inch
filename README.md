<p align="center">
    <a href="./images/logo_colour.png">
        <img src="images/logo_colour.png" width="500">
    </a>
</p>

# INCH - the Interactive Nuclear CHart

---

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/php1ic/inch.svg)](https://github.com/php1ic/inch/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/php1ic/inch.svg)](https://github.com/php1ic/inch/pulls)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![codecov](https://codecov.io/gh/php1ic/inch/branch/master/graph/badge.svg)](https://codecov.io/gh/php1ic/inch)
[![CodeFactor](https://www.codefactor.io/repository/github/php1ic/inch/badge)](https://www.codefactor.io/repository/github/php1ic/inch)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/12ec340c7c694de58f3fc431b5861bcb)](https://www.codacy.com/manual/php1ic/inch?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=php1ic/inch&amp;utm_campaign=Badge_Grade)

---

| OS      | Config                                      | Build Status                                                                                                                                                      |   Compiler Version |
|---------|---------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------:|
| Linux   | [Gitlab](.gitlab-ci.yml)                    | [![Pipeline Status](https://gitlab.com/php1ic/inch/badges/master/pipeline.svg)](https://gitlab.com/php1ic/inch/pipelines)                                         |     gcc10, clang10 |
|         | [CircleCI](.circleci/config.yml)            | [![CircleCI](https://circleci.com/gh/php1ic/inch/tree/master.svg?style=svg)](https://circleci.com/gh/php1ic/inch/tree/master)                                     |       gcc9, clang9 |
|         | [Github](.github/workflows/cmake_build.yml) | [![BuildAndTest](https://github.com/php1ic/inch/workflows/BuildAndTest/badge.svg)](https://github.com/php1ic/inch/actions)                                        |       gcc9, clang9 |
| Windows | [Appveyor](appveyor.yml)                    | [![Build status](https://ci.appveyor.com/api/projects/status/8q5tr9o3htxmqsbq/branch/master?svg=true)](https://ci.appveyor.com/project/php1ic/inch/branch/master) | MSVC2017, MSVC2019 |


## Introduction

INCH is a C++ project aimed at providing a simple method of creating high quality (preferably vector graphic) images of a bespoke region of the nuclear, sometimes referred to as Segre, chart.

No guarantee is supplied with regards to the accuracy of the data presented.
Estimated values are included, please always refer to the original sources.
All data should, however, be accurate.

Suggestions/improvements are encouraged, either create an [issue](https://github.com/php1ic/inch/issues) or implement the change and send a pull request via [github](https://github.com/php1ic/inch) .

The code can read data from the following published papers:
- The Atomic Mass Evaluation
  - [2003](http://www.sciencedirect.com/science/article/pii/S0375947403018086) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.002)
  - [2012](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2709.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/002)
  - [2016](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8344.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030002)
- The NUBASE evaluation of nuclear and decay properties
  - [2003](http://www.sciencedirect.com/science/article/pii/S0375947403018074) - [DOI](http://dx.doi.org/10.1016/j.nuclphysa.2003.11.001)
  - [2012](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract2725.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/36/12/001)
  - [2016](http://cpc-hepnp.ihep.ac.cn:8080/Jwk_cpc/EN/abstract/abstract8343.shtml) - [DOI](http://dx.doi.org/10.1088/1674-1137/41/3/030001)

The data contained within is freely available as ASCII files, all of which are included with the project for ease.
The newer tables are available on the [AMDC](https://www-nds.iaea.org/amdc/) website, and the [historic](http://amdc.in2p3.fr/) page can be used to find older ones.


## Output file formats and what is drawn

As it currently stands, the available outputs, and what they can do are:

| Functionality                                         | [Encapsulated Postscript (*.eps)][EPS]   | [LaTeX/PGF/TikZ][TikZ] | [Scalable Vector Graphics (*.svg)][SVG]   |
| :---------------------------------------------------- | :--------------------------------------- | :-------------------   | :---------------------------------------- |
| Create and position a shape representing an isotope   | :heavy_check_mark:                       | :heavy_check_mark:     | :heavy_check_mark:                        |
| Add text to the icon representing an isotope          | :heavy_check_mark:                       | :heavy_check_mark:     | :x:                                       |
| Show magic numbers                                    | :heavy_check_mark:                       | :x:                    | :x:                                       |
| Show R-process                                        | :heavy_check_mark:                       | :x:                    | :x:                                       |
| Show key                                              | :heavy_check_mark:                       | :x:                    | :x:                                       |
| Show drip lines                                       | :heavy_check_mark:                       | :x:                    | :x:                                       |

[EPS]:https://en.wikipedia.org/wiki/Encapsulated_PostScript
[SVG]:https://www.w3.org/TR/SVG/
[TIKZ]:https://www.ctan.org/pkg/pgf

## Aquiring

Simplest way to aquire the repo is to clone it.
The project makes use of [fmt](https://github.com/fmtlib/fmt) and [Catch2](https://github.com/catchorg/Catch2) via submodules so they need to be initialised.
The below is enough to get you started.

```
$ git clone https://github.com/php1ic/inch.git
$ cd inch
$ git submodule init
$ git submodule update --recursive
```


## Building

The project is built using [cmake](https://cmake.org/) and instructions on building can be found in the file [INSTALL.md](INSTALL.md).

The following files, within *./data_files/*, should **NOT** be altered.
Although they are all freely available from the above links if they are modified.

```
FRLDM_ME.tbl
mass.mas03
mass.mas12
mass16.txt
nubtab03.asc
nubase.mas12
nubase2016.txt
```

Any other files in *./data_files/* are used for extra functionality and in some cases are recreated if not present.
As always, if you aren't sure, don't change it and if you do, backup!


## Versioning
There is an attempt to follow [SemVer](http://semver.org/) for versioning numbers.
For the currently available versions, see the [tags on this repository](https://github.com/php1ic/inch/tags).


## License
The code may be freely copied, distributed and altered under the terms of the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html) license.
Please read the file [COPYING](COPYING), which should be located at the root of the project, for full details.

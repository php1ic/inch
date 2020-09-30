# Changelog
All notable changes to this project will be documented in this file.

There is an attempt to adhere to [Semantic Versioning](https://semver.org/).

## [Unreleased](https://github.com/php1ic/inch/tree/master)

### Added
- Questions asked to user now have digits, rather then characters, as options
- Unit testing with [catch2](https://github.com/catchorg/Catch2) (v2.13.1)
- Use the [{fmt}](https://github.com/fmtlib/fmt) library (v7.0.3) to print to screen and write to file
- CircleCI support
- Documentation with [doxygen](http://www.doxygen.nl/)
- Compile against C++17
- Github Actions workflow
- Measure testing code coverage and report to [codecov](https://codecov.io)
- Ability to output all isotopic data as a single csv or json file. Watch this space for a python implementation (pynch?)

### Removed
- The hand crafted GNUMakefile. This was lagging at C++11 and didn't build unit tests or doxygen. Too much effort to maintain.

### Fixes
- Symbol string was not being capitalise as expected in the Converter class.
- Converting float to string was not correctly stripping out decimal places and the decimal point if non were required.
- If required, the grid was drawn before the chart size was drawn so had zero size.
- Fixed setting of state member, Pass by reference was changed to by value, so underlying member was not set.
- Running on windows didn't work as filepath manipulation assumed linux filesystem. Use of regex fixes issue.
- Not quite a fix, but the extractValue() function has been removed in favour of the std::sto{i,f} variants. On my machine (i7-2670QM@301GHz/8GBRAM), averaged over 1000 runs using gcc (10.1.1) and clang (10.0.0), this gave the following changes in run time:
  |         | Run time [seconds] |
  |:--------|-------------------:|
  | Debug   | 0.07 -> 0.05 (30%) |
  | Release | 0.04 -> 0.02 (50%) |


## [[0.9.8]](https://github.com/php1ic/inch/releases/tag/v0.9.8) - 2018-02-10

Lots has happened since [0.9.7](https://github.com/php1ic/inch/releases/tag/v0.9.7) and there has been no attempt at versioning.
I'm not even going to try and list the all the changes, more accurate book keeping will start with the next release.

Major things are:
- Move to cmake for building
- Compile against C++14
- CI support for gitlab, travis and appveyor
- Embrace OOP, remove all free functions and refactor functionality into separate classes
- All drawing options can now be set via the command line


## [[0.9.7]](https://github.com/php1ic/inch/releases/tag/v0.9.7) - 2014-05-12

This was mainly learning how to tag rather than any specific milestone.

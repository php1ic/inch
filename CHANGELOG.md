# Changelog
All notable changes to this project will be documented in this file.

There is an attempt to adhere to [Semantic Versioning](https://semver.org/).

## [Unreleased](https://github.com/php1ic/inch/tree/master)

### Added
- Unit testing with [catch2](https://github.com/catchorg/Catch2)
- CircleCI support
- Documentation with [doxygen](http://www.doxygen.nl/)

### Fixes
- Symbol string was not being capitalise as expected in the Converter class.
- Converting float to string was not correctly stripping out decimal places and the decimal point if non were required.
- If required, the grid was drawn before the chart size was drawn so had zero size.
- Fixed setting of state member, Pass by reference was changed to by value, so underlying member was not set.

## [[0.9.8]](https://github.com/php1ic/inch/releases/tag/v0.9.8) - 2018-02-10

Lots has happened since [0.9.7](https://github.com/php1ic/inch/releases/tag/v0.9.7) and and there has been no attempt at versioning.
I'm not even going to try and list the all the changes, more accurate book keeping will start with the next release.

Major things are:
- Move to cmake for building
- Compile against C++14
- CI support for gitlab, travis and appveyor
- Embrace OOP, remove all free functions and refactor functionality into separate classes
- All drawing options can now be set via the command line


## [[0.9.7]](https://github.com/php1ic/inch/releases/tag/v0.9.7) - 2014-05-12

This was mainly learning how to tag rather than any specific milestone.

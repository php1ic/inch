# INCH Installation

## [GNU Make](https://www.gnu.org/software/make/)

From the root of the project, running `make` will build the executable in the *./bin/* directory. This is all that is required to build the project.

As part of the build, the binary is coded to read files located in */your/build/path/inch/data_files/*. Thus the executable can be copied into one of your $PATH directories and it will still be able to find the necessary files.

There is no `make install` currently implemented.


## [cmake](https://cmake.org/)

For a cmake build, in-source builds are blocked so compilation needs to be done in a separate build directory. Assuming we are currently in the root of the project, the following will build the executable.

```bash
cd ../ && mkdir build && cd build
cmake ../inch
make
```

This will create a *bin/* directory (inside the build directory) and place the executable inside.

The option `CMAKE_INSTALL_PREFIX` can be passed to cmake to specify the install location if required.

As part of the build, the binary is coded to read files located in */your/build/path/inch/data_files/*. Thus the executable can be copied into one of your $PATH directories and it will still be able to find the necessary files.


## Additional Scripts

There are a scripts provided with the project that are designed to aid with conversion from the currently output file format to one which may be easier to use in presentations or documents, and enable some basic end-to-end testing of functionality.

All but one of the scripts are written in both [BASH](https://www.gnu.org/software/bash/) and [Python](https://www.python.org/). Functionality should be identical between the two so use which ever you prefer.

### File type conversion

The scripts [convert_chart.sh](convert_chart.sh) and [convertChart.py](convertChart.py) convert the output file from one format to another. The implemented conversions are listed in the table below, running with no arguments will print usage instructions to remind you of the allowed conversions

|From|To           |Package utilised                                   |
|--- |---          |---                                                |
|eps |png, jpg, pdf|[ghostscript](https://www.ghostscript.com/)        |
|svg |pdf          |[librsvg2](https://wiki.gnome.org/Projects/LibRsvg)|


### Create a random chart

The scripts [random_chart.sh](random_chart.sh) and [randomChart.py](randomChart.py) will run the code using a random Z range, choice of experimental or theoretical isotopes and property to colour by. Currently the full neutron range is always used.

The script assumes you are in the same location from which you built the executable so will look for the binary in paths relatvie to that. If this is not the case, you can pass the correct path with the `-e` flag. The number of charts to create, each with their own options, can be set with the `-n` flag.


### Check your postscript

The scripts [validate_epsfile.sh](validate_epsfile.sh) and [validateEPSfile.py](validateEPSfile.py) silently passes the provided file through ghostscript to check for postscript syntax errors. The error message is not currently displayed, or saved, but rather a simple PASS/FAIL string is printed along with the filename being read at the time. Files will need to be processed individually to run further debugging.

One or more argument is required so wildcards can be used to pass multiple files.

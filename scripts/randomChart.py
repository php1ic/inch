#!/usr/bin/env python3
"""
Create a chart or charts with random parameters,
using either the executable provided
"""

import argparse
import os
import sys
import colorama
import subprocess
import string
import random


def getExecutableName():
    """
    Get the name of the executable that is going to be used.

    @param: None

    @return: The basename of the executable that will be used
    """
    programName = "inch"

    currentdir = os.getcwd()
    scriptdir = os.path.realpath(__file__)

    os.chdir(os.path.dirname(scriptdir))

    try:
        output = subprocess.run(["git", "rev-parse", "--show-toplevel"],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)

        programName = os.path.basename(output.stdout.strip().decode())
    except OSError as e:
        if e.errno == os.errno.ENOENT:
            print("Looks like git is not installed on this system")
        else:
            raise

    os.chdir(currentdir)
    return programName
#-------------------------------------------------


def validateExecutable(exe):
    """
    Check the provided executable exists, otherwise look for it in the
    'standard' locations

    @param: File path

    @return[success]: Path to valid executable
    @return[failure]: None
    """
    if exe is not None:
        if os.path.isfile(exe):
            return exe
        else:
            print(colorama.Fore.YELLOW + "WARNING: " + colorama.Style.RESET_ALL
                  + exe + " does not exist."
                  + " Looking for executable in \"usual\" build locations")

    exeName = None
    fullExe = None

    exeName = getExecutableName()

    commonPath = os.path.join("bin", exeName)
    gnumakePath = os.path.abspath(os.path.join(".", commonPath))
    cmakePath = os.path.abspath(os.path.join("..", "..", "build", commonPath))

    colorama.init()
    if os.path.isfile(gnumakePath):
        fullExe = gnumakePath
    elif os.path.isfile(cmakePath):
        fullExe = cmakePath
    else:
        print(colorama.Fore.RED + "ERROR: " + colorama.Style.RESET_ALL
              + " Couldn't find an executable to use")

    colorama.deinit()
    return fullExe
#-------------------------------------------------


def runExecutable(exe,number):
    """
    Run <exe> <number> times, randomising the input parameters

    @param: Executable to run
    @param: Number of times to run <exe>

    @return: Nothing
    """
    #We could read this from ../includes/inputs.h
    MAX_Z = 118
    #Set the minimum possible Z range
    MAX_LOW_Z = MAX_Z - 1

    colorama.init()
    print("\nUsing: "
          + colorama.Fore.GREEN + exe + colorama.Style.RESET_ALL
          + " to create "
          + colorama.Fore.GREEN + str(number) + colorama.Style.RESET_ALL
          + " chart(s)\n")
    colorama.deinit()

    for i in range(0,number):
        # Randomly pick from a,b,c
        exprimental = random.choice(string.ascii_lowercase[0:2])

        # If the experimental option is 'b' i.e theoretical, there is one less property to colour by
        # Randomly pick form a,b,c,d and possibly e
        if exprimental is not "b":
            choice = random.choice(string.ascii_lowercase[0:4])
        else:
            choice = random.choice(string.ascii_lowercase[0:3])

        min = random.randrange(MAX_LOW_Z)

        max = min + random.randrange(MAX_Z - min)

        name = "Zmin-{:03d}_Zmax-{:03d}_Exp-{}_Type-{}".format(min, max, exprimental, choice)

        print("Creating - {}".format(name))

        with open(name+".in",'w') as f:
            f.write("section=b\n"
                    + "Zmin={}\n".format(min)
                    + "Zmax={}\n".format(max)
                    + "required=a\n"
                    + "type={}\n".format(exprimental)
                    + "choice={}\n".format(choice))

        f.close()

        subprocess.run([exe, "-o" , name, "-i", name+".in"],
                       stdout=subprocess.DEVNULL,
                       stderr=subprocess.DEVNULL)
#-------------------------------------------------


def check_positive(value):
    """
    Check that the value is positive while also converting to an int.
    Use to ensure that the number of charts option make sense.

    @param: Number

    @return[success]: The integer version of the number
    @return[failure]: ArgparseTypeError
    """
    intValue = int(value)
    if intValue <= 0:
        raise argparse.ArgumentTypeError("{} is an invalid positive int value".format(intValue))

    return intValue
#-------------------------------------------------


def parse_arguments():
    """
    Encapsulate the use of argparse

    @param: None

    @return: An instance of argparse
    """
    parser = argparse.ArgumentParser(description="Create some random charts")

    # Required
    #Nothing

    # Optional
    parser.add_argument("-e", "--executable",
                        help="The executable to use [default: None]",
                        type=str,
                        default=None)

    parser.add_argument("-n", "--number",
                        help="Number of charts to randomly create [default: 1]",
                        type=check_positive,
                        default=1)

    return parser.parse_args()
#-------------------------------------------------


if __name__ == "__main__":

    args = parse_arguments()

    exe = validateExecutable(args.executable)

    if exe is not None:
        runExecutable(exe, args.number)
#-------------------------------------------------

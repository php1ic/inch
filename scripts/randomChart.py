#!/usr/bin/env python3
"""
Create chart(s) with random parameters,
using either the executable provided, or
searching for it in standard locations,
and farm the creation on multiple threads
"""

import argparse
import multiprocessing
import os
import random
import string
import subprocess

import colorama

from joblib import Parallel, delayed


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
# -------------------------------------------------


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

    exeName = getExecutableName()
    scriptdir = os.path.realpath(os.path.dirname(__file__))

    commonPath = os.path.join("bin", exeName)
    gnumakePath = os.path.abspath(os.path.join(scriptdir, "..", commonPath))
    cmakePath = os.path.abspath(os.path.join(scriptdir, "..", "..", "build", commonPath))

    fullExe = None

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
# -------------------------------------------------


def createSingleChart(MAX_LOW_Z, MAX_Z):
    """
    Generate a single chart with random parameters. Limit the Z range
    to [MAX_LOW_Z,MAX_Z]

    @param: Highest value of Z to use as Zmin
    @param: Largest value of Z allowed

    @return: Nothing
    """
    # This script removes the ability to interact with the program so need to make sure
    # that the file we are try to create does not already exist. Otherwise the script will
    # get stuck waiting for a user input that will never come
    while True:
        # Randomly pick from a,b,c
        exprimental = random.choice(string.ascii_lowercase[0:3])

        # If the experimental option is 'b' i.e theoretical, there is one less property
        # to colour by so randomly pick form a,b,c,d and possibly e
        choice = random.choice(string.ascii_lowercase[0:4 if exprimental is "b" else 5])

        min = random.randrange(MAX_LOW_Z)

        max = min + random.randrange(MAX_Z - min)

        name = "Zmin-{:03d}_Zmax-{:03d}_Exp-{}_Type-{}".format(min, max, exprimental, choice)

        if not os.path.isfile(name+".eps"):
            break

    print("Creating - {}".format(name))

    with open(name+".in", 'w') as f:
        f.write("section=b\n"
                + "Zmin={}\n".format(min)
                + "Zmax={}\n".format(max)
                + "required=a\n"
                + "type={}\n".format(exprimental)
                + "choice={}\n".format(choice))

    f.close()

    subprocess.run([exe, "-o", name, "-i", name+".in"],
                   stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)
# -------------------------------------------------


def runExecutable(exe, number, threads):
    """
    Run <exe> <number> times, randomising the input parameters.
    Each execution is independent so allow <exe> to be run over <threads> threads

    @param: Executable to run
    @param: Number of times to run <exe>
    @param: Number of threads to concurrently use

    @return: Nothing
    """
    # We could read this from ../includes/inputs.h
    MAX_Z = 118
    # Set the minimum possible Z range
    MAX_LOW_Z = MAX_Z - 1

    colorama.init()
    print("\nUsing: "
          + colorama.Fore.GREEN + exe + colorama.Style.RESET_ALL
          + " to create "
          + colorama.Fore.GREEN + str(number) + colorama.Style.RESET_ALL
          + " chart(s)\n")
    colorama.deinit()

    Parallel(threads)(delayed(createSingleChart)(MAX_LOW_Z, MAX_Z) for i in range(0, number))

    print()
# -------------------------------------------------


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
# -------------------------------------------------


def parse_arguments():
    """
    Encapsulate the use of argparse

    @param: None

    @return: An instance of argparse
    """
    parser = argparse.ArgumentParser(description="Create some random charts")

    # Required
    # Nothing

    # Optional
    parser.add_argument("-e", "--executable",
                        help="The executable to use [default: None]",
                        type=str,
                        default=None)

    parser.add_argument("-n", "--number",
                        help="Number of charts to randomly create [default: %(default)s]",
                        type=check_positive,
                        default=1)

    parser.add_argument("-t", "--threads",
                        help="Number of threads to use [default: %(default)s]",
                        type=int,
                        default=multiprocessing.cpu_count()-1,
                        choices=range(1, multiprocessing.cpu_count()))

    return parser.parse_args()
# -------------------------------------------------


if __name__ == "__main__":

    args = parse_arguments()

    exe = validateExecutable(args.executable)

    if exe is not None:
        runExecutable(exe, args.number, args.threads)
# -------------------------------------------------

#!/usr/bin/env python3
"""
Create chart(s) with random parameters

Using either the executable provided, or
searching for it in standard locations,
and farm the creation on multiple threads
"""

import argparse
import multiprocessing
import os
import random
import shutil
import subprocess

import colorama

from joblib import Parallel, delayed


def getExecutableName():
    """
    Get the name of the executable that is going to be used.

    @param: None

    @return[success] The basename of the executable that will be used
    @return[failure] The default value
    """
    # Set a default program name incase this function fails
    programName = "inch"

    # Store where we are so we can comeback
    currentdir = os.getcwd()

    # Get the path of this script
    scriptdir = os.path.realpath(__file__)
    # Move into the script directory as it's guaranteed to part of the git repo
    os.chdir(os.path.dirname(scriptdir))

    # Use git to get the repo directory name, assume this is also the exe name
    gitExe = shutil.which("git")

    if gitExe is None:
        print("Looks like git is not installed on this system")
        print(f"Using the default {programName} as the executable name")
        return programName

    output = subprocess.run([gitExe, "rev-parse", "--show-toplevel"],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)

    programName = os.path.basename(output.stdout.strip().decode())

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
    colorama.init()
    if exe is not None:
        if os.path.isfile(exe):
            return exe

        print(colorama.Fore.YELLOW + "WARNING: " + colorama.Style.RESET_ALL
              + f"{exe} does not exist."
              "Looking for executable in standard build locations")

    exeName = getExecutableName()
    scriptdir = os.path.realpath(os.path.dirname(__file__))

    commonPath = os.path.join("bin", exeName)
    gnumakePath = os.path.abspath(os.path.join(scriptdir, "..", commonPath))
    cmakePath = os.path.abspath(os.path.join(scriptdir, "..", "..", "build", commonPath))

    fullExe = None

    if os.path.isfile(gnumakePath):
        fullExe = gnumakePath
    elif os.path.isfile(cmakePath):
        fullExe = cmakePath
    else:
        print(colorama.Fore.RED + "ERROR: " + colorama.Style.RESET_ALL
              + f" Couldn't find an executable to use")

    colorama.deinit()
    return fullExe
# -------------------------------------------------


def createSingleChart(MAX_LOW_Z, MAX_Z):
    """
    Generate a single chart with random parameters.
    Limit the Z range to [MAX_LOW_Z,MAX_Z]

    @param: Highest value of Z to use as Zmin
    @param: Largest value of Z allowed

    @return: Nothing
    """
    # This script removes the ability to interact with the program so need to make sure
    # that the file we are try to create does not already exist. Otherwise the script will
    # get stuck waiting for a user input that will never come
    while True:
        # Randomly pick 0,1,2
        experimental = random.choice(range(0, 3))

        # If the experimental option is '1' i.e theoretical, there is one less property
        # to colour by so randomly pick form a,b,c,d and possibly e
        choice = random.choice(range(0, 4 if experimental == 1 else 5))

        minZ = random.randrange(MAX_LOW_Z)

        maxZ = minZ + random.randrange(MAX_Z - minZ)

        name = f"Zmin-{minZ:03d}_Zmax-{maxZ:03d}_Exp-{experimental}_Type-{choice}"

        if not os.path.isfile(name+".eps"):
            break

    print(f"Creating - {name}")

    with open(name+".in", 'w') as ofile:
        ofile.write(f"section=1\n"
                    f"Zmin={minZ}\n"
                    f"Zmax={maxZ}\n"
                    f"required=0\n"
                    f"type={experimental}\n"
                    f"choice={choice}\n")

    ofile.close()

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
    print(f"\nUsing: "
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
        raise argparse.ArgumentTypeError(f"{intValue} is an invalid positive int value")

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
    colorama.init()

    args = parse_arguments()

    exe = validateExecutable(args.executable)

    if exe is not None:
        runExecutable(exe, args.number, args.threads)

    colorama.deinit()
# -------------------------------------------------

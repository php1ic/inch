#!/usr/bin/env python3
"""
Validate that the given file contains legal postscript syntax

This script is bascially a wrapper around ghostscript
"""

import argparse
import multiprocessing
import os
import shutil
import subprocess

import colorama

from joblib import Parallel, delayed


def CheckType(file):
    """
    Confirm that the given file has an eps extension and that it exists

    @param: File to check

    @return[success]: The original file name
    @return[failure]: A TypeError is raised
    """
    if not os.path.isfile(file):
        raise argparse.ArgumentTypeError(f"Inputfile <{file}> cannot be found")

    if not file.endswith('.eps'):
        raise argparse.ArgumentTypeError(f"InputFile must be of type .eps")

    return file
# -------------------------------------------------


def parse_arguments():
    """
    Parse the commandline arguements

    @param: None

    @return: An instance of argparse
    """
    parser = argparse.ArgumentParser(description="Validate the file(s)")

    # Required
    parser.add_argument("epsfiles",
                        nargs='+',
                        help="Files to validate",
                        default=[],
                        type=CheckType)

    # Optional
    parser.add_argument("-t", "--threads",
                        help="Number of threads to use [default: %(default)s]",
                        type=int,
                        default=multiprocessing.cpu_count()-1,
                        choices=range(1, multiprocessing.cpu_count()))

    return parser.parse_args()
# -------------------------------------------------


def validateSingleFile(file):
    """
    Use ghostscript to validate <file>

    @param: File to validate

    @return: Nothing
    """
    validator = "gs"
    validationCMD = shutil.which(validator)

    if validationCMD is None:
        print(f"***ERROR***: Validation failed: {validator} is not available")
        return

    returnval = subprocess.run(
        [validationCMD, "-o", "/dev/null", "-sDEVICE=nullpage", "-dQUIET", "-sstderr=%stdout", file],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

    if returnval.returncode:
        colour = colorama.Fore.RED + "FAIL" + colorama.Style.RESET_ALL
    else:
        colour = colorama.Fore.GREEN + "PASS" + colorama.Style.RESET_ALL

    print(f"[{colour}] - {file}")
# -------------------------------------------------


def validateFiles(threads, FileList):
    """
    Use ghostscript to validate the eps files provided

    @param: The number of threads to use to validate files simultaneously
    @param: A list of eps files

    @return: Nothing
    """
    colorama.init()
    Parallel(threads)(delayed(validateSingleFile)(file) for file in FileList)
    colorama.deinit()
# -------------------------------------------------


if __name__ == "__main__":
    args = parse_arguments()

    validateFiles(args.threads, args.epsfiles)
# -------------------------------------------------

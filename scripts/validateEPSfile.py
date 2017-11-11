#!/usr/bin/env python3
"""
Validate that the given file contains legal postscript syntax

This script is bascially a wrapper around ghostscript
"""

import argparse
import os
import subprocess
import colorama


def CheckType(file):
    """
    Confirm that the given file has an eps extension and that it exists

    @param: File to check

    @return[success]: The original file name
    @return[failure]: A TypeError is raised
    """
    if not file.endswith('.eps'):
        raise argparse.ArgumentTypeError('InputFile must be of type .eps')
    elif not os.path.isfile(file):
        raise argparse.ArgumentTypeError("Inputfile <{}> cannot be found".format(file))

    return file
#-------------------------------------------------


def parse_arguments():
    """
    Parse the commandline arguements

    @param: None

    @return: An instance of argparse
    """
    parser = argparse.ArgumentParser(description="Validate the file(s)")

    parser.add_argument("epsfiles",
                        nargs='+',
                        help="Files to validate",
                        default=[],
                        type=CheckType)

    return parser.parse_args()
#-------------------------------------------------


def validateFiles(FileList):
    """
    Use ghostscript to validate the eps files provided

    @param: A list of eps files

    @return: Nothing
    """
    colorama.init()
    for file in FileList:
        returnval = subprocess.run(
            ["gs", "-o", "/dev/null", "-sDEVICE=nullpage", "-dQUIET", "-sstderr=%stdout", file],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )

        if returnval.returncode:
            colour = colorama.Fore.RED + "FAIL" + colorama.Style.RESET_ALL
        else:
            colour = colorama.Fore.GREEN + "PASS" + colorama.Style.RESET_ALL

        print("[" + colour + "] - {}".format(file))

    colorama.deinit()
#-------------------------------------------------


if __name__ == "__main__":
    args = parse_arguments()

    validateFiles(args.epsfiles)
#-------------------------------------------------

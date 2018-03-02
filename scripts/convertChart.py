#!/usr/bin/env python3
"""
File conversion script

Currently ghostscript and rsvg-convert are used to convert so only the
following conversions are possible:
 eps -> pdf,png,jpg
 svg -> pdf
"""

import argparse
import os
import subprocess


def CheckType(file):
    """
    Confirm that the given file has an eps or svg extension and that it exists

    @param: File to check

    @return[success]: The original file name
    @return[failure]: A TypeError is raised
    """
    if not file.endswith(('.eps', '.svg')):
        raise argparse.ArgumentTypeError('InputFile must be of type .eps or .svg')
    elif not os.path.isfile(file):
        raise argparse.ArgumentTypeError("Inputfile <{}> cannot be found".format(file))

    return file
# -------------------------------------------------


def GetBoundingBox(infile):
    """
    Extract the upper (x,y) values of the eps bounding box

    @param: An eps file

    @return: The x and y values of the top right corner of the bounding box
    """
    for line in open(infile, 'r'):
        if "%%BoundingBox: 0 0" in line:
            return line.split()[-2:]
# -------------------------------------------------


def ConvertSVG(infile, outputfile, filetype='pdf'):
    """
    Convert the given svg file to the specified outfile

    @param: A svg file
    @param: The outputfile

    @return: Nothing
    """
    print ("Converting {} -> {}".format(infile, outputfile))

    try:
        subprocess.run([
            "rsvg-convert",
            "-f", filetype,
            "-o", outputfile,
            infile]
        )
    except OSError as e:
        print("***ERROR***: Conversion failed: {}".format(e.strerror))
# -------------------------------------------------


def ConvertEPS(infile, outputfile, resolution):
    """
    Convert the given eps file to the specified outfile with resolution

    @param: The eps file
    @param: The outputfile
    @param: The resolution of the outputfile

    @return: Nothing
    """
    filetype = os.path.splitext(outputfile)[1]
    print ("Converting {} -> {}".format(infile, outputfile))

    x, y = GetBoundingBox(infile)

    GHOSTSCRIPT_OPTIONS = ([
        "-dBATCH",
        "-dNOPAUSE",
        "-dSAFER"
    ])

    if filetype == ".pdf":
        device = "pdfwrite"
    elif filetype == ".jpg" or filetype == ".png":
        DPI = "-r" + str(resolution)
        GHOSTSCRIPT_OPTIONS += (["-dTextAlphaBits=4", "-dGraphicsAlphaBits=4", DPI])

        print("with a resolution of {} dpi".format(resolution))

        if filetype == ".jpg":
            device = "jpeg"
        elif filetype == ".png":
            device = "png16m"

    OUTDEVICE = "-sDEVICE=" + device
    OUTFILE = "-sOutputFile=" + outputfile
    PAGE = "<< /PageSize [" + x + " " + y + "] >> setpagedevice"

    try:
        subprocess.run(
            ["gs"]
            + GHOSTSCRIPT_OPTIONS
            + [OUTDEVICE,
               OUTFILE,
               "-c", PAGE,
               "-f", infile],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
    except OSError as e:
        print("***ERROR***: Conversion failed: {}".format(e.strerror))
# -------------------------------------------------


def ConvertFile(args):
    """
    High level function to do a more detailed passing of the arguments
    and call the relevant conversion function

    @param: An instance of argparse, contain the give options

    @return: Nothing
    """
    absolutefilename, fileextension = os.path.splitext(args.InputFile)
    outputfilename = absolutefilename + '.' + args.OutputFileType

    # Can we do this check with argparse?
    if fileextension == ".svg" and args.OutputFileType != "pdf":
        print("Conversion from svg -> pdf is not currently implemented")
        parser.print_help()
        sys.exit(1)

    if fileextension == ".svg":
        ConvertSVG(args.InputFile, outputfilename)
    elif fileextension == ".eps":
        ConvertEPS(args.InputFile, outputfilename, args.resolution)
# -------------------------------------------------


def parse_arguments():
    """
    Encapsulate the use of argparse

    @param: None

    @return: An instance of argparse
    """
    parser = argparse.ArgumentParser(description="Convert file")

    # Required
    parser.add_argument("InputFile",
                        help="File to convert",
                        type=CheckType)

    parser.add_argument("OutputFileType",
                        help="File type to convert to",
                        choices=['jpg', 'png', 'pdf'])

    # Optional
    parser.add_argument("-r", "--resolution",
                        help="dpi resolution of the converted image [default is 500]",
                        type=int,
                        default=500)

    return parser.parse_args()
# -------------------------------------------------


if __name__ == "__main__":

    args = parse_arguments()

    ConvertFile(args)
# -------------------------------------------------

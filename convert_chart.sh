#!/bin/bash

usage() {
    echo -e "
\tUSAGE: $(basename $0) file.eps [png,jpg,pdf]
\t   OR: $(basename $0) file.svg pdf
"
    exit -1
}

if [[ $# -ne 2 ]]
then
    echo -e "\n\tWARNING: You need to give a file to be converted, and the format to be converted to\n"
    usage
fi

inputfile=$1

if [[ ! -e ${inputfile} ]]
then
    echo -e "\n\tERROR: ${inputfile} does not exist\n"
    exit -1
fi

name=${inputfile%.*}
ext=${inputfile##*.}
filetype=$2

if [[ ${ext} != "eps" && ${ext} != "svg" ]]
then
    echo -e "\n\tERROR: Input file, ${inputfile} is neither eps nor svg file\n"
    usage
elif [[ ${filetype} != "png" && ${filetype} != "jpg" && ${filetype} != "pdf" ]]
then
    echo -e "\n\tERROR: The script cannot currently convert to $filetype format\n"
    usage
fi

outputfile=${name}.${filetype}

if [[ ${ext} == "eps" ]]
then
    read x y <<< $(grep "^%%BoundingBox: 0 0" ${inputfile} | awk '{print $4,$5}')
fi

########################################################
# Changing this number drastically alters the file size.
#
RESOLUTION=300

GS_OPTIONS="-dBATCH -dNOPAUSE -dSAFER -dTextAlphaBits=4"

echo -e "\nConverting ${inputfile} -> ${outputfile}"

if [[ ${filetype} == "pdf" ]]
then
    if [[ ${ext} == "eps" ]]
    then
	DEVICE=pdfwrite
    else
	rsvg-convert -f ${filetype} -o ${outputfile} ${inputfile}
	exit $?
    fi
elif [[ ${ext} != "svg" ]]
then
    echo "with a resolution of ${RESOLUTION} dpi"
    GS_OPTIONS+=" -dGraphicsAlphaBits=4 -r${RESOLUTION}"

    if   [[ ${filetype} == "png" ]]; then DEVICE=png16m
    elif [[ ${filetype} == "jpg" ]]; then DEVICE=jpeg
    fi
fi

#Option order is important/critical, do not change
gs ${GS_OPTIONS} -sDEVICE=${DEVICE} -sOutputFile=${outputfile} -c "<< /PageSize [$x $y]  >> setpagedevice" -f ${inputfile} 2>&1 > /dev/null

echo ""

exit $?

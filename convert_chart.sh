#!/bin/bash

RESTORE="\e[0m" #N.B. This is a reset of the colour, not black
RED="\e[31m"
GREEN="\e[32m"
BLUE="\e[34m"
YELLOW="\e[33m"

usage() {
    echo -e "
\t${BLUE}USAGE:${RESTORE} $(basename "$0") file.eps [png,jpg,pdf] <-r DPI_RESOLUTION>
\t   ${BLUE}OR:${RESTORE} $(basename "$0") file.svg pdf
"
    exit -1
}

if [[ $# -lt 2 ]]
then
    echo -e "\n\t${YELLOW}WARNING:${RESTORE} You need to give a file to be converted, and the format to be converted to"
    usage
fi

inputfile=$1

if [[ ! -e ${inputfile} ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} ${inputfile} does not exist"
    usage
    exit -1
fi

name=${inputfile%.*}
ext=${inputfile##*.}
filetype=$2

if [[ ${ext} != "eps" && ${ext} != "svg" ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} Input file, ${inputfile} is neither eps nor svg file\n"
    usage
elif [[ ${filetype} != "png" && ${filetype} != "jpg" && ${filetype} != "pdf" ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} The script cannot currently convert to $filetype format\n"
    usage
fi

outputfile=${name}.${filetype}

if [[ ${ext} == "eps" ]]
then
    read -r x y <<< "$(awk '/^%%BoundingBox: 0 0/ {print $4,$5}' "${inputfile}")"
fi

GS_OPTIONS="-dBATCH -dNOPAUSE -dSAFER -dTextAlphaBits=4"

echo -e "\n${GREEN}Converting${RESTORE} ${inputfile} ${GREEN}->${RESTORE} ${outputfile}"

if [[ ${filetype} == "pdf" ]]
then
    if [[ ${ext} == "eps" ]]
    then
	DEVICE=pdfwrite
    else
	rsvg-convert -f "${filetype}" -o "${outputfile}" "${inputfile}"
	exit $?
    fi
elif [[ ${ext} != "svg" ]]
then
    args=( $@ )

    for (( r=0; r<$#; r++ ))
    do
        if [[ "${args[$r]}" == "-r" ]]
        then
            RESOLUTION=${args[$r+1]}
        fi
    done

    # Changing this number drastically alters the file size.
    RESOLUTION=${RESOLUTION:-300}

    echo -e "${GREEN}with a resolution of${RESTORE} ${RESOLUTION} ${GREEN}dpi${RESTORE}\n"
    GS_OPTIONS+=" -dGraphicsAlphaBits=4 -r${RESOLUTION}"

    if   [[ ${filetype} == "png" ]]; then DEVICE=png16m
    elif [[ ${filetype} == "jpg" ]]; then DEVICE=jpeg
    fi
fi

#Option order is important/critical, do not change
gs ${GS_OPTIONS} \
   -sDEVICE=${DEVICE} \
   -sOutputFile="${outputfile}" \
   -c "<< /PageSize [$x $y]  >> setpagedevice" \
   -f "${inputfile}" \
   > /dev/null 2>&1

exit $?

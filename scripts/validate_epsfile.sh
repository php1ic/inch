#!/bin/bash

RESTORE="\e[0m" #N.B. This is a reset of the colour, not black
RED="\e[31m"
GREEN="\e[32m"
BLUE="\e[34m"
YELLOW="\e[33m"

if [[ $# -lt 1 ]]
then
    echo -e "\n\t${BLUE}USAGE:${RESTORE} ${0##*/} file01.eps file02.eps ... fileN.eps\n"
    exit 1
fi

while [[ "$1" != "" ]]
do
    inputfile=$1

    if [[ ${inputfile##*.} != "eps" ]]
    then
        echo -e "[${YELLOW}ERROR${RESTORE}] - ${inputfile} is not an eps"
        shift
        continue
    fi

    if gs -o /dev/null -sDEVICE=nullpage -dQUIET -sstderr=%stdout "${inputfile}" > /dev/null 2>&1
    then
        echo -e "[${GREEN}PASS${RESTORE}] - ${inputfile}"
    else
        echo -e "[${RED}FAIL${RESTORE}] - ${inputfile}"
    fi

    shift
done

exit $?

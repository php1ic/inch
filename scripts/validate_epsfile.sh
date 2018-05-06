#!/bin/bash
# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/text_colours.sh

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

    # Use the return value to check if file is valid so output and redirect everything to /dev/null
    if gs -o /dev/null -sDEVICE=nullpage -dQUIET -sstderr=%stdout "${inputfile}" > /dev/null 2>&1
    then
        echo -e "[${GREEN}PASS${RESTORE}] - ${inputfile}"
    else
        echo -e "[${RED}FAIL${RESTORE}] - ${inputfile}"
    fi

    shift
done

exit $?

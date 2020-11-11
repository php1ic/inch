#!/usr/bin/env bash

set -euo pipefail

# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

usage() {
    echo -e "
\t${BLUE}USAGE:${RESTORE} ${BASH_SOURCE##*/} <-e path to executable>
"
    exit 1
}

while getopts ":he:" OPTIONS
do
    case "${OPTIONS}" in
        h | \? | : )
            usage
            ;;
        e )
            #Default = empty/NULL
            exe=${OPTARG}
            ;;
    esac
done

# If no executable was provided, look in some sensible places
if [[ -z "${exe}" ]]
then
    exe=$(locateEXE)
fi

if [[ ${exe} == "EMPTY" ]]
then
    exit 1
fi

declare -A FILES=( [0]=FullError [1]=RelativeError [2]=DecayMode [3]=HalfLife [4]=Isomer)

for file in "${!FILES[@]}"
do
    OUTFILE=fullChart_${FILES[$file]}.eps

    echo -e "${GREEN}Creating${RESTORE} - ${OUTFILE}"
    if [ -f  "${OUTFILE}" ]
    then
        echo -e "${OUTFILE} exists, deleting"
        rm "${OUTFILE}"
    fi

    ${exe} -o "${OUTFILE}" <<< "0
2
${file}" > /dev/null

done

exit $?

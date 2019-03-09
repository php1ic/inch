#!/usr/bin/env bash

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

declare -A FILES=( [a]=FullError [b]=RelativeError [c]=DecayMode [d]=HalfLife [e]=Isomer)

for file in "${!FILES[@]}"
do
    OUTFILE=fullChart_${FILES[$file]}.eps

    echo -e "${GREEN}Creating${RESTORE} - ${OUTFILE}"
    if [ -f  "${OUTFILE}" ]
    then
        echo -e "${OUTFILE} exists, deleting"
        rm "${OUTFILE}"
    fi

    ${exe} -o "${OUTFILE}" <<< "a
c
${file}" > /dev/null

done

exit $?

#!/usr/bin/env bash

# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/text_colours.sh

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
    if ! command -v git >/dev/null 2>&1
    then
        program=$(basename "$(git rev-parse --show-toplevel)")
    else
        program=inch
    fi

    if [[ -x "${scriptdir}/../bin/${program}" ]]
    then
        #echo "Looks like you have built with GNU Make"
        exe=$(readlink -f "${scriptdir}/../bin/${program}")
    elif [[ -x "${scriptdir}/../../build/bin/${program}" ]]
    then
        #echo "Looks like you have built with cmake"
        exe=$(readlink -f "${scriptdir}/../../build/bin/${program}")
    else
        echo -e "\n\t${RED}ERROR${RESTORE}: No executable ${program} in either of:"
        echo -e "\t\t$(dirname "$(readlink -m "${scriptdir}/../../build/bin/${program}")")"
        echo -e "\t\t$(dirname "$(readlink -m "${scriptdir}/../bin/${program}")")"
        echo -e "Exiting...\n"
        exit 1
    fi
fi

declare -A FILES=( [a]=FullError [b]=RelError [c]=DecayMode [d]=HalfLife [e]=Isomer)

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

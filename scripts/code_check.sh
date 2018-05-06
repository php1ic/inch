#!/bin/bash
# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/text_colours.sh

#Set the analyser to use
CODECHECKER=cppcheck

#Check it's actually installed
if ! command -v ${CODECHECKER} >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: Code check tool is set to ${CODECHECKER} but it isn't installed\n"
    exit 1
fi

#Use absolute paths
rootdir="$(git rev-parse --show-toplevel)"
sources="${rootdir}/src"
includes="${rootdir}/include"

#Run the analysis
${CODECHECKER} \
    --enable=all \
    --suppress=missingIncludeSystem \
    -I"${includes}" \
    "${sources}"/*.cpp

exit $?

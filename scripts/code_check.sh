#!/bin/bash

#Set the analyser to use
CODECHECKER=cppcheck

#Check it's actually installed
if ! command -v ${CODECHECKER} >/dev/null 2>&1
then
    echo -e "\n\t***ERROR***: Code check tool is set to ${CODECHECKER} but it isn't installed\n"
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

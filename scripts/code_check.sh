#!/bin/bash

rootdir=$(git rev-parse --show-toplevel)

sources="${rootdir}/src"
includes="${rootdir}/include"

cppcheck \
    --enable=all \
    --suppress=missingIncludeSystem \
    -I"${includes}" \
    "${sources}"/*.cpp

exit $?

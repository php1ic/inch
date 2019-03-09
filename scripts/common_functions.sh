#!/usr/bin/env bash

# Define colours here and use in scripts, rather than
# redefing in any all all scripts that need colour

# Reset
RESTORE="\e[0m"

BLACK="\e[0;30m"
RED="\e[0;31m"
GREEN="\e[0;32m"
YELLOW="\e[0;33m"
BLUE="\e[0;34m"
PURPLE="\e[0;35m"
CYAN="\e[0;36m"
WHITE="\e[0;37m"

# Look in some sensible places for an executable
locateEXE() {
    scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
    # Assume the name is that of the project,
    # otherwise it's the name of the project this script was written for
    if ! command -v git >/dev/null 2>&1
    then
        program=$(basename "$(git rev-parse --show-toplevel)")
    else
        program=inch
    fi

    exe="EMPTY"

    # Paths relative to the scripts/ directory
    declare -a localpaths=("../build/bin" \
                               "../../build/bin"
                          )

    # Convert the relative paths to absolute and look for the exe
    for path in "${localpaths[@]}"
    do
        location="${scriptdir}/${path}"

        if [[ -x "${location}" ]]
        then
            exe=$(readlink -f "${location}/${program}")
            break
        fi
    done

    # If we didn't find the exe print a message saying so
    if [[ ${exe} == "EMPTY" ]]
    then
        echo -e "\n\t${RED}ERROR${RESTORE}: No executable ${program} in any of:"
        for path in "${localpaths[@]}"
        do
            location="${scriptdir}/${path}"

            echo -e "\t\t$(dirname "$(readlink -m "${location}")")\n"
        done
        echo -e "Exiting...\n"
    fi

    # output the full path of the program or the string "empty"
    echo "${exe}"
}

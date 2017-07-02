#!/bin/bash

numruns=1
exe=

while [[ "$1" != "" ]]
do
    case $1 in
        -n | --numruns )
            shift
            numruns=$1
            ;;
        -e | --executable )
            shift
            exe=$1
            ;;
    esac

    shift

done

scriptdir=$(dirname "$(readlink -f "$0")")

if [[ -z "${EXE}" ]]
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
        echo -e "\nNo executable ${program} in either of:"
        echo -e "\t$(dirname "$(readlink -m "${scriptdir}/../../build/bin/${program}")")"
        echo -e "\t$(dirname "$(readlink -m "${scriptdir}/../bin/${program}")")"
        echo -e "Exiting...\n"
        exit 1
    fi
fi

# The character 'a' when printing an int as a char
char_offset=97
# How many display options are there
display_options=3
# How many different properties are there to colour by
property_options=5
# The current highest Z value possible
max_z=118
# Set the upper limit on Zmin
# N.B. Currently, setting max_low_z = max_z may cause a divide by zero error
max_low_z=$((max_z-5))

# Create the data for a random chart and execute the program ${numruns} times
for ((i=0; i<"${numruns}"; i++))
do
    exp=$(printf \\$(printf '%03o' $((char_offset + RANDOM%display_options))))

    # If the display option is 'b' i.e theoretical, there is one less property to colour by
    options=$([[ "${exp}" != "b" ]] && echo property_options || echo $((property_options-1)))

    type=$(printf \\$(printf '%03o' $((char_offset + RANDOM%options))))

    min=$((RANDOM%max_low_z))

    remaining=$((max_z-min))

    max=$((min + RANDOM%remaining))

    name=Zmin-$(printf "%03d" ${min})_Zmax-$(printf "%03d" ${max})_Exp-${exp}_Type-${type}

    echo "Creating - ${name}"

    ${exe} -o "${name}" <<< "b
${min}
${max}
a
${exp}
${type}" > /dev/null && mv options.in "${name}.in"

done

exit $?

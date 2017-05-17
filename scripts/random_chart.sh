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
    if [[ ! $(type -P git) ]]
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


for ((i=0; i<"${numruns}"; i++))
do
    exp=$(printf \\$(printf '%03o' $((97 + RANDOM%3))))

    min=$((RANDOM%100))

    remaining=$((118-MIN))

    max=$((min + RANDOM%remaining))

    if [[ ${exp} == "b" ]]
    then
        type=$(printf \\$(printf '%03o' $((97 + RANDOM%4))))
    else
        type=$(printf \\$(printf '%03o' $((97 + RANDOM%5))))
    fi

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

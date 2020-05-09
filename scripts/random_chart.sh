#!/usr/bin/env bash

# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/common_functions.sh

usage() {
    echo -e "
\t${BLUE}USAGE:${RESTORE} ${BASH_SOURCE##*/} -n NUMBER_OF_CHARTS <-e path to executable>
"
    exit 1
}

#Default to 1 chart if the number is not set
numruns=1

while getopts ":hn:e:" OPTIONS
do
    case "${OPTIONS}" in
        h | \? | : )
            usage
            ;;
        n )
            #Default = 1
            numruns=${OPTARG}
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

echo -e "\nUsing: ${GREEN}${exe}${RESTORE} to create ${GREEN}${numruns}${RESTORE} charts\n"

# How many display options are there
display_options=3
# How many different properties are there to colour by
property_options=5
# The current highest Z value possible
max_z=118
# Set the upper limit on Zmin
# N.B. Currently, setting max_low_z = max_z may cause a divide by zero error
max_low_z=$((max_z-1))

if [[ ${max_low_z} -ge ${max_z} ]]
then
    lower=5
    echo -e "\n\t${YELLOW}WARNING${RESTORE}: The variable max_low_z (${max_low_z}) must be less than max_z (${max_z})"
    echo -e "\t         Setting max_low_z to ${lower} less than max_z\n"
    max_low_z=$((max_z-lower))
fi

# Create the data for a random chart and execute the program ${numruns} times
for ((i=0; i<"${numruns}"; i++))
do
    exp=$((RANDOM%display_options))

    # If the display option is '1' i.e theoretical, there is one less property to colour by
    options=$([[ "${exp}" != "1" ]] && echo property_options || echo $((property_options-1)))

    type=$((RANDOM%options))

    min=$((RANDOM%max_low_z))

    remaining=$((max_z-min))

    max=$((min + RANDOM%remaining))

    name=Zmin-$(printf "%03d" ${min})_Zmax-$(printf "%03d" ${max})_Exp-${exp}_Type-${type}

    echo "Creating - ${name}"

    # Run the command and save the options used
    ${exe} -o "${name}" <<< "1
${min}
${max}
0
${exp}
${type}" > /dev/null

done

echo ""

exit $?

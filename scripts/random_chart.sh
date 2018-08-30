#!/usr/bin/env bash
# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/text_colours.sh

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

echo -e "\nUsing: ${GREEN}${exe}${RESTORE} to create ${GREEN}${numruns}${RESTORE} charts\n"

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
    exp=$(printf \\$(printf '%03o' $((char_offset + RANDOM%display_options))))

    # If the display option is 'b' i.e theoretical, there is one less property to colour by
    options=$([[ "${exp}" != "b" ]] && echo property_options || echo $((property_options-1)))

    type=$(printf \\$(printf '%03o' $((char_offset + RANDOM%options))))

    min=$((RANDOM%max_low_z))

    remaining=$((max_z-min))

    max=$((min + RANDOM%remaining))

    name=Zmin-$(printf "%03d" ${min})_Zmax-$(printf "%03d" ${max})_Exp-${exp}_Type-${type}

    echo "Creating - ${name}"

    # Run the command and save the options used
    ${exe} -o "${name}" <<< "b
${min}
${max}
a
${exp}
${type}" > /dev/null

done

echo ""

exit $?

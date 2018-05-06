#!/bin/bash
# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source ${scriptdir}/text_colours.sh

#We go wrong in a number of ways so create this function for simple reuse
usage() {
    echo -e "
\t${BLUE}USAGE:${RESTORE} ${0##*/} -i file.eps -o [png,jpg,pdf] <-r DPI_RESOLUTION>
\t   ${BLUE}OR:${RESTORE} ${0##*/} -i file.svg -o pdf
"
    exit 1
}

while getopts ":hi:o:r:" OPTIONS
do
    case "${OPTIONS}" in
        h | \? | : )
            usage
            ;;
        i )
            inputfile=${OPTARG}
            ;;
        o )
            filetype=${OPTARG}
            ;;
        r )
            resolution=${OPTARG}
            ;;
    esac
done

#Check the minimum arguments were supplied
if [[ -z ${inputfile} || -z ${filetype} ]]
then
    echo -e "\n\t${YELLOW}WARNING:${RESTORE} You need to give a file to be converted, and the format to be converted to"
    usage
#Check the input file actually exists
elif [[ ! -e ${inputfile} ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} ${inputfile} does not exist"
    usage
fi

#Extract the base of the file name and the extension
name=${inputfile%.*}
ext=${inputfile##*.}

#Validate the input and output file types
if [[ ${ext} != "eps" && ${ext} != "svg" ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} Input file ${inputfile} is neither eps nor svg file"
    usage
elif [[ ${filetype} != "png" && ${filetype} != "jpg" && ${filetype} != "pdf" ]]
then
    echo -e "\n\t${RED}ERROR:${RESTORE} The script cannot currently convert to $filetype format"
    usage
fi

#Construct the file name to be created
outputfile="${name}.${filetype}"

#Extract the size if the input is an eps file
if [[ ${ext} == "eps" ]]
then
    read -r xsize ysize <<< "$(awk '/^%%BoundingBox: 0 0/ {print $4,$5}' "${inputfile}")"
fi

#These options are required no matter what conversion is done
gsoptions="-dBATCH -dNOPAUSE -dSAFER"

echo -e "\nConverting ${GREEN}${inputfile}${RESTORE} -> ${GREEN}${outputfile}${RESTORE}"

if [[ ${ext} == "svg" ]]
then
    #Currently svg->pdf is the only conversion so this is an attempt at future proofing
    if [[ ${filetype} == "pdf" ]]
    then
        rsvg-convert -f "${filetype}" -o "${outputfile}" "${inputfile}"
        exit $?
    fi
elif [[ ${ext} == "eps" ]]
then
    if [[ ${filetype} == "pdf" ]]
    then
        device=pdfwrite
    elif [[ ${filetype} == "png" || ${filetype} == "jpg" ]]
    then
        #Use the value provided, otherwise fallback to the default
        #Changing this number drastically alters the file size.
        resolution=${resolution:-300}

        echo -e "with a resolution of ${GREEN}${resolution}${RESTORE} dpi"
        #Additional jpg/png options
        gsoptions+=" -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -r${resolution}"

        case ${filetype} in
            "png" )
                device=png16m
                ;;
            "jpg" )
                device=jpeg
                ;;
        esac
    fi
fi

echo ""
#Option order is important/critical, do not change
gs ${gsoptions} \
   -sDEVICE=${device} \
   -sOutputFile="${outputfile}" \
   -c "<< /PageSize [${xsize} ${ysize}]  >> setpagedevice" \
   -f "${inputfile}" \
   > /dev/null 2>&1

exit $?

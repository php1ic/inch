#!/bin/bash

if [[ $# -ne 2 ]]
then
    echo -e "
You need to give an eps file to be converted,
and the format to be converted to

\tUSAGE: $0 file.eps [png,jpg,pdf]
\t   OR: $0 file.svg pdf
"
    exit -1
fi

name=${1%.*}
ext=${1##*.}
file=$2

if [[ $ext != "eps" && $ext != "svg" ]]
then
    echo -e "
\tERROR: Input file is not an eps or svg file
"
    exit -1
fi

x=`grep '^%%Bou.*: 0' $1 | awk '{print $4}'`
y=`grep '^%%Bou.*: 0' $1 | awk '{print $5}'`

########################################################
# The number after r is the resolution so can
# be changed for higher quality conversions.
# Changing this number drastically alters the file size.
#
r=300

if [[ $file != "png" && $file != "jpg" && $file != "pdf" ]]
then
    echo -e "
\tERROR: The script cannot currently convert to $file format

\tUSAGE: $0 file.eps [png,jpg,pdf]
\t   OR: $0 file.svg pdf
"
elif [ $file == "png" ]
then
    echo -e "
Converting $1 -> $name.png
with a resolution of $r dpi
"
    gs -dBATCH -dNOPAUSE -dSAFER -r$r -dTextAlphaBits=4 -dGraphicsAlphaBits=4 \
	-sOutputFile=$name".png" -sDEVICE=png16m \
	-c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
elif [ $file == "jpg" ]
then
    echo -e "
Converting $1 -> $name.jpg
with a resolution of $r dpi
"
    gs -dBATCH -dNOPAUSE -dSAFER -r$r -dTextAlphaBits=4 -dGraphicsAlphaBits=4 \
	-sOutputFile=$name".jpg" -sDEVICE=jpeg \
	-c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
elif [ $file == "pdf" ]
then
    if [[ $ext == "eps" ]]
    then
	echo -e "\nConverting $1 -> $name.pdf\n"
	gs -dBATCH -dNOPAUSE -dSAFER -dTextAlphaBits=4 \
	    -sOutputFile=$name".pdf" -sDEVICE=pdfwrite \
	    -c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
    else
	rsvg-convert -f pdf -o $name.pdf $name.svg
    fi
fi

exit $?

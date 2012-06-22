#!/bin/bash

if [[ $# -ne 2 ]]
then
    echo -e "
You need to give an eps file to be converted,
and the format to be converted to

\tUSAGE: $0 file.eps [png,jpg,pdf]
"
    exit -1
fi

name=${1%.*}
ext=${1##*.}
file=$2

if [[ $ext != "eps" ]]
then
    echo -e "
\tERROR: Input file is not an eps file
"
    exit -1
fi

x=`grep '^%%Bou.*: 0' $1 | awk '{print $4}'`
y=`grep '^%%Bou.*: 0' $1 | awk '{print $5}'`

#################################################
#
# The number after r is the resolution so can be changed for higher quality conversions.
# Changing this number drastically alters the file size.
#

if [[ $file != "png" && $file != "jpg" && $file != "pdf" ]]
then
    echo -e "
\tERROR: The script cannot currently convert to $file format

\tUSAGE: $0 file.eps [png,jpg,pdf]
"
elif [ $file == "png" ]
then
    r=300
    echo -e "
Converting $1 -> $name.png
with a resolution of $r dpi
"
    gs -dBATCH -dNOPAUSE -r$r -dTextAlphaBits=4 -dGraphicsAlphaBits=4 \
	-sOutputFile=$name".png" -sDEVICE=png16m \
	-c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
elif [ $file == "jpg" ]
then
    r=300
    echo -e "
Converting $1 -> $name.jpg
with a resolution of $r dpi
"
    gs -dBATCH -dNOPAUSE -r$r -dTextAlphaBits=4 -dGraphicsAlphaBits=4 \
	-sOutputFile=$name".jpg" -sDEVICE=jpeg \
	-c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
elif [ $file == "pdf" ]
then
    echo -e "\nConverting $1 -> $name.pdf\n"
    gs -dBATCH -dNOPAUSE -dTextAlphaBits=4 \
	-sOutputFile=$name".pdf" -sDEVICE=pdfwrite \
	-c "<< /PageSize [$x $y]  >> setpagedevice" -f $1 2>&1 > /dev/null
fi

exit $?

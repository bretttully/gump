#! /bin/sh

DIRECTORIES="src/hytrac src/oxutil src/oftin testing/hytrac testing/oxutil testing/oftin"
CPP_FILES=`find $DIRECTORIES -name "*.cpp"`
H_FILES=`find $DIRECTORIES -name "*.h"`
files="${CPP_FILES} ${H_FILES}"

mkdir -p out

for item in $files ; do
    if [ "$item" = "src/oftin/material_tables.h" ]
    then
        echo "Skipping file $item"
        continue
    fi

    dn=$(dirname $item)
    mkdir -p out/$dn
    uncrustify -f $item -c "./uncrustify.cfg" -o indentoutput.tmp
    mv indentoutput.tmp "$item"

done


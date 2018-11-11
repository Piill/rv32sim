#!/bin/bash

FOLDERS="./task1"

for d in $FOLDERS ; do
    cd $d
    for o in *.out
    do
        control=${o/.out/.control}
        res=$(diff $o $control)
        if [[ !  -z  $res ]]
        then
            echo "$o : $control failed"
            echo $res
        fi
    done
    cd ..
done




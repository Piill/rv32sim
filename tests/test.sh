#!/bin/bash

FOLDERS="./task1 ./task2 ./task3 ./instruction_tests "

for d in $FOLDERS ; do
    cd $d
    for o in *.out
    do
        control=${o/.out/.res}
        res=$(diff -q <(xxd $o) <(xxd $control))
        if [[ !  -z  $res ]]
        then
            echo -e "$o \t failed"
        else
            echo -e "$o \t passed"
        fi
    done
    cd ..
done




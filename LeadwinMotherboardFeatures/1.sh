#!/bin/bash

flag=1
result=1

sleep 10s
/home/root/arm_main &

while [ "$flag" -eq 1 ]
do
    sleep 1s
    result=`pidof arm_main`
    if [ -z "$result" ]; then
        /home/root/arm_main &
        echo"process is finished"
    fi
done



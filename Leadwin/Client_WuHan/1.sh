#!/bin/bash

flag=1
result=1
rm /home/root/yiyuan_4g.log
rm /home/root/arm-WuHan_main.log
/etc/yuyuan_4g >/home/root/yiyuan_4g.log &
sleep 10s
/home/root/arm-WuHan_main >/home/root/arm-WuHan_main.log &

while [ "$flag" -eq 1 ]
do
    sleep 1s
    result=`pidof arm-WuHan_main`
    if [ -z "$result" ]; then
        /home/root/arm-WuHan_main >/home/root/arm-WuHan_main.log &
        echo"process is finished"
    fi
done



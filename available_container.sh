#!/bin/bash

source watch_setting.sh

for (( i=1; i<=$con_count; i++ ))
do
	cmd=`docker inspect -f {{.State.Running}} $con$i`
	if [ $cmd = "false" ]
	then
		echo $i
		exit
	fi
done

echo -1

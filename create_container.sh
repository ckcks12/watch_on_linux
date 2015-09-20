#!/bin/bash

source watch_setting.sh

for ((i=1;i<=$count;i++))
do
	echo $conname$i
	docker run -itd --name $con_name$i --cap-add SYS_PTRACE --security-opt apparmor:unconfined --net=none $image_name | exit
done

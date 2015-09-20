#!/bin/bash

source watch_setting.sh

con=$con$1

rm -f $out$i
docker start $con
docker cp $src$i $con:/source.c
docker exec $con /bin/bash -c "rm -f /out /a.out && /watch"
docker cp $con:/out $out

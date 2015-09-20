#!/bin/bash

source watch_setting.sh

con=$con$1
src=src$1
out=src$1

rm -f $out
docker start $con
docker cp $src $con:/source.c
docker exec $con /bin/bash -c "rm -f /out /a.out && /watch"
docker cp $con:/out $out

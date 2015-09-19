#!/bin/bash

con="watcher2"
src="/source.c"
out="/out"

rm -f $out
docker stop $con
docker start $con
docker cp $src $con:/source.c
docker exec $con /bin/bash -c "rm -f /out /a.out && /watch"
docker cp $con:/out $out

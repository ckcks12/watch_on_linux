# watch_on_linux

For your security, please run in docker container.
It needs SYS_PTRACE capabilty and apparmor:unconfined because it uses ptrace to watch.

These things what it supports.
###1. Elapsed time limit
Elapsed time is very important thing that prevent from endless loop. It checks time by `clock_gettime();`.
###2. CPU usage time limit
It will prevent your cpu from high CPU usage and CPU overheating. I'm not sure on `sleep(2147483647);`. You guys check this for me and fork me :) See ya!
###3. Writing file size limit
To avoid creating data It restricts the data size able to be written. I'm really love it. I know you do too.

...
I'm feeling blue tonight.
~~I'm gonna do away with me.~~
♬ I am firefly ~~~ 

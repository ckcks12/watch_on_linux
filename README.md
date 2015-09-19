# watch_on_linux

For your security, please run in docker container.
It needs SYS_PTRACE capability and apparmor:unconfined because it uses ptrace to watch.

The watch_on_linux features.
###1. Elapsed time limit
Elapsed time limit is very important thing that prevents from endless loop. It checks time by `clock_gettime();`.
###2. CPU usage time limit
It will prevent your CPU from high CPU usage and CPU overheating. I'm not sure it works on `sleep(2147483647);`. You guys check this for me and fork me :) See ya!
###3. Writing file size limit
To avoid creating huge data It restricts the data size able to be written. I'm really love it. I know you do too.
###4. Input from file and output to file.
I think it's not special feature.

<img src="https://pbs.twimg.com/profile_images/476169263305457665/I8QlqUVh.jpeg" width=100 height=100>
It is made by fu@cking tiny university student—that's me. WTF!

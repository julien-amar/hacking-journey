# Process structure (Linux)

As everything is a file on Linux, all process informations can be acceced through the pseudo-file system in `/proc/{pid}`

Through this folder, all the process relative information can be retrieved, such as:
* network (net)
* file descriptors (fd)
* memory map (maps)
* threads
* environment variables (environ)
* arguments (cmdline)

For more details: https://man7.org/linux/man-pages/man5/proc.5.html

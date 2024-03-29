# Linux

## Everything is a file

On linux, everything is represented as a file descriptor: https://en.wikipedia.org/wiki/Everything_is_a_file

## Standard Streams

On linux, when a process is executed, it already has three predefined streams open and available for use:
* stdin (0), used as standard program input.
* stdout (1), used as standard program output.
* stderr (2), used as standard program error output.

# Terminal

## Useful commands
```
man <command>           Show the manual page of specified <command>

ls [-l] [-a]            Show the content of the current/working location
cd <path>               Change current/working directory to <path>
pwd                     Show the current/working directory
mkdir <name>            Create a new directory named <name>
rm [-r] <path>          Remove files or directories
chmod +<perm> <path>    Add the specific permission to the path
file <file>             Determine <file> type
cat <path>              Show the content of the file at <path>
touch <name>            Create a new file named <name> / Change file timestamps

echo <text>             Display <text>
uname [-a]              Print system information

free [-h]               Display amount of free and used memory in the system
df [-h]                 Report file system disk space usage
du [-sh] <folders>      Show how much space folders take
ps [aux]                Report a snapshot of the current processes
pstree -aClp            Report the process/thread tree (with command line)
lsof <file>             Show processes that open a given file

id                      Print real and effective user and group IDs

hexdump                 Display data in hex/oct/ascii/dec format

ldd <binary>            Print shared object dependencies

netstat -ac             Print network connections continuously
netstat -pl             Print listening network connections with associated process identifier. 
```

An interactive MAN exploration solution is available here: https://explainshell.com/

## Package manager

Each Linux distribution use a different package maanger. Distributions based on debian (like Ubuntu, Kali, ...) use `apt`.

To update `apt` based distribution, you need to uptdate your package cache and then upgrade your packages:
```sh
$> sudo apt-get update
$> sudo apt-get upgrade
```

## Bash

Bash  is  a sh-compatible command language interpreter that executes commands read from the standard input or from a file.

When it starts, it automatically load the following files:
* /etc/bash.bashrc
* ~/.bashrc
* /etc/profile
* ~/.profile

### PATH variable

To locate executable binaries the shell use the `PATH` environment variable.

You can add additionnal location to look for binaries (using the `export` built-in): `export  PATH=$PATH:/home/bin`

`whereis <command> ` enables to locate a specific binary, source, and manual page files.

## VIM basic shortbuts

`vim -O <file> <file>` enables to open multiple files splitted verticaly

```
i                   Insertion mode
<esc>               Quit insertion mode
:w                  Write the file
:q                  Quit the editor
:wq                 Write & quit the editor
:syntax on          Enable syntax highlighting
:set number         Show line numbers in the editor

<d><d>              Remove the current line
<y><y>              Copy the current line to the clipboard
<p>                 Paste the content of the clipboard.

:!<command>         Execute the command

<shift>+<O>         Insert
<shift>+<G>         Go to the end of the file

If editing multiple files:
<Ctrl>+<W>+<Left>   Navigate to Left panel
<Ctrl>+<W>+<Right>  Navigate to Right panel
```
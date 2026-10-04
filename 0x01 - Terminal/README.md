# Linux

## Everything is a file

On Linux, almost everything is represented as a file (or, more precisely, as a
*file descriptor*): regular files, directories, devices, sockets, and even
running processes (through `/proc`). This uniform interface is why the same
tools (`cat`, `read`, `write`) work on so many different things.

Reference: <https://en.wikipedia.org/wiki/Everything_is_a_file>

## Standard streams

When a process starts, three streams are already open and available:

* **stdin** (file descriptor `0`) — standard input (what you type / pipe in).
* **stdout** (file descriptor `1`) — standard output (normal results).
* **stderr** (file descriptor `2`) — standard error (error/diagnostic messages).

Keeping errors on a separate stream lets you redirect them independently:

```sh
./program > out.txt 2> errors.txt   # stdout to one file, stderr to another
./program > all.txt 2>&1            # merge stderr into stdout, both to all.txt
./program 2>/dev/null               # discard errors
cat data.txt | ./program            # feed a file into stdin via a pipe
```

# Terminal

## Useful commands

```
man <command>           Show the manual page of the specified <command>

ls [-l] [-a]            List the contents of the current/given directory (-l long, -a hidden)
cd <path>               Change the current/working directory to <path>
pwd                     Print the current/working directory
mkdir [-p] <name>       Create a directory (-p also creates parent directories)
rm [-r] <path>          Remove files or directories (-r for directories)
cp [-r] <src> <dst>     Copy files or directories
mv <src> <dst>          Move or rename files
chmod +<perm> <path>    Add a permission to the path (see chapter 0x09)
file <file>             Determine a file's type (based on its magic bytes)
cat <path>              Print the content of a file
less <path>             View a file one screen at a time (q to quit)
touch <name>            Create an empty file / update a file's timestamp

echo <text>             Print <text>
uname [-a]              Print system information (-a for everything)

free [-h]               Show free and used memory (-h = human-readable sizes)
df [-h]                 Show filesystem disk-space usage
du [-sh] <folders>      Show how much space folders take (-s summary, -h readable)
ps [aux]                Snapshot of running processes (aux = all, with details)
pstree -aClp            Show the process/thread tree (with command lines)
lsof <file>             List processes that have a given file open
top / htop              Live, interactive view of processes and resource usage

id                      Print the real/effective user and group IDs
whoami                  Print the current username

hexdump [-C] <file>     Display data in hex/ascii (-C = canonical hex+ascii)
xxd <file>              Hex dump (handy with -r to go back from hex to binary)

ldd <binary>            Print a binary's shared-library dependencies

netstat -ac             Continuously print network connections
netstat -plnt           List listening TCP ports with the owning process (needs root)
ss -plnt                Modern, faster replacement for netstat
```

### Searching and filtering

These are some of the most useful commands for day-to-day work and for CTFs:

```
grep [-r] [-i] <pattern> <path>   Search for a pattern in files (-r recursive, -i ignore case)
grep -n <pattern> <file>          Also show matching line numbers
find <path> -name "<glob>"        Find files by name
find <path> -type f -perm -4000   Find setuid binaries (classic privesc hunt)
which <command>                   Show the path of a command found in $PATH
wc [-l] <file>                    Count lines/words/bytes (-l = lines only)
sort / uniq                       Sort lines / collapse duplicate lines
cut -d<delim> -f<n>               Extract column <n> from delimited text
tr <set1> <set2>                  Translate or delete characters
sed 's/old/new/g' <file>          Stream editor: find/replace, delete, etc.
awk '{print $2}' <file>           Field-based text processing
```

Pipe them together to build quick one-liners, e.g. "the 5 most common IPs in a
log":

```sh
awk '{print $1}' access.log | sort | uniq -c | sort -rn | head -5
```

An interactive way to explore any command line is available here:
<https://explainshell.com/>

## Package manager

Each Linux distribution uses a different package manager. Distributions based on
Debian (Ubuntu, Kali, …) use `apt`.

To update an `apt`-based distribution, refresh the package cache and then upgrade
the installed packages:

```sh
sudo apt-get update      # refresh the list of available packages
sudo apt-get upgrade     # upgrade installed packages to the latest versions
sudo apt-get install <package>   # install a new package
```

## Bash

Bash is an `sh`-compatible command-language interpreter that executes commands
read from standard input or from a file.

When it starts, it automatically loads several configuration files (which ones
depend on whether the shell is a login or interactive shell):

* `/etc/bash.bashrc`
* `~/.bashrc`
* `/etc/profile`
* `~/.profile`

These are a common place to look (and sometimes to persist access) because any
command placed in them runs whenever a shell starts.

### PATH variable

To locate executable binaries, the shell uses the `PATH` environment variable —
a colon-separated list of directories searched in order.

You can add a location to search (with the `export` built-in):

```sh
export PATH=$PATH:/home/bin
```

`whereis <command>` locates a command's binary, source, and manual-page files.
`which <command>` shows exactly which binary would run for a given name.

> Note: a writable directory early in someone's `PATH` is a classic privilege-
> escalation vector — if you can place a malicious binary there, it may run in
> place of the real command.

## Vim basic shortcuts

Vim is modal: you are either in *normal* mode (keys are commands) or *insert*
mode (keys are text). Press `<Esc>` to return to normal mode.

`vim -O <file> <file>` opens multiple files split vertically.

```
i                   Enter insert mode
<Esc>               Leave insert mode (back to normal mode)
:w                  Write (save) the file
:q                  Quit the editor
:q!                 Quit without saving
:wq                 Write and quit
:syntax on          Enable syntax highlighting
:set number         Show line numbers

dd                  Delete (cut) the current line
yy                  Copy (yank) the current line
p                   Paste the clipboard after the cursor
u                   Undo
<Ctrl>+r            Redo
/pattern            Search forward for pattern (n = next, N = previous)

:!<command>         Run a shell command without leaving Vim

<Shift>+o           Open a new line above and enter insert mode
<Shift>+g           Go to the end of the file
gg                  Go to the start of the file

If editing multiple files:
<Ctrl>+w then <Left>    Move to the left split
<Ctrl>+w then <Right>   Move to the right split
```

> If you are new to Vim, run `vimtutor` in a terminal for a 30-minute hands-on
> introduction.

# Forensic

## Memory dump

### Volatility

[Volatility](https://github.com/volatilityfoundation/volatility3) is one of the best open source software programs for analyzing RAM in 32 bit/64 bit systems.
It supports analysis for Linux, Windows, Mac, and Android systems.

```
volatility -f <dump> imageinfo                                          Dump information
volatility -f <dump> --profile <profile> printkey -K <key>              Retrieve registry key from memory dump
volatility -f <dump> --profile <profile> hashdump                       Retrieve LM & NTLM hashes
volatility -f <dump> --profile <profile> hivelist                       Retrieve cached hives
volatility -f <dump> --profile <profile> pslist                         Retrieve process list
volatility -f <dump> --profile <profile> -p <pid> procdump --dump-dir=. Dump a specific process
volatility -f <dump> --profile <profile> netscan                        Retrieve active network connections
volatility -f <dump> --profile <profile> cmdline                        Retrieve command line arguments
volatility -f <dump> --profile <profile> consoles                       Retrieve console's command history
volatility -f <dump> --profile <profile> filescan                       Retrieve all opened files
volatility -f <dump> --profile <profile> dumpfiles -D . -Q <offset>     Dump opened file to current directory
volatility -f <dump> --profile <profile> truecryptsummary               Retrieve truecrypt artifacts
```

Source: https://repository.root-me.org/Forensic/EN%20-%20Volatility%20cheatsheet%20v2.4.pdf

#### Plugins

* Firefox history: https://github.com/superponible/volatility-plugins
```
volatility —plugins=/usr/share/volatility/contrib/plugins/ -f dump —profile=<profile> firefoxhistory > ff_history.txt
```

### binwalk

[Binwalk](https://github.com/ReFirmLabs/binwalk) is a tool for searching a given binary image for embedded files and executable code.

```
binwalk -e <file> <output_folder>       Automatically extract known files that are part of input file.
```

Package name: `sudo apt install binwalk`

## Disk dump

### Sleuthkit

[Sleuthkit](https://sleuthkit.org/) is a collection of command line tools and a C library that allows you to analyze disk images and recover files from them.

```
mmls <dump>                                             Display the partition layout of a volume system
fls -o <partition_offset> -r <dump>                     List file and directory names in a disk image
fls -o <partition_offset> -r <dump> | grep \*           List removed files in a disk image
icat -o <partition_offset> -r <dump> <inode> > file     Output the contents of a file based on its inode number
```

As a alternative, you can use `dd`:
```
dd if=<dump> of=output.disk skip=<partition_offset>
mkdir mount_point
mount -t vfat ./output.disk mount_point/
cd mount_point/
```

Package name: `sudo apt install sleuthkit`

## File meta-data

### EXIF

**exif** is a small command-line utility to show and change EXIF information (GPS informations, etc.) in JPEG files.

An online version of such tool is available: https://exifdata.com/

```
exif <jpg_file>     Shows EXIF information in JPEG files
```

Package name: `sudo apt install exif`

### Office (DOC/XLS/PPT)

[olevba](https://github.com/decalage2/oletools/wiki/olevba) is a script to parse OLE and OpenXML files such as MS Office documents (e.g. Word, Excel), to detect VBA Macros, extract their source code in clear text, and detect security-related patterns such as auto-executable macros, suspicious VBA keywords used by malware, anti-sandboxing and anti-virtualization techniques, and potential IOCs (IP addresses, URLs, executable filenames, etc).

### Others

As a more generic tool, to retrieve file meta data (for Office, PDF, pictures & archives files), you can use: https://www.extractmetadata.com/

## Device/Network analysis

### Network

```
tshark -2 -r <capture>.pcap -R "<wireshark_filter>" -T fields -e <field>        Export data from a PCAP file.
```

Package name: `sudo apt install tshark`

### Device

#### Keyboard

A script is available in this repository (`./devices/keyboard/keylogger-reader.c`), to parse keyboard capture file.

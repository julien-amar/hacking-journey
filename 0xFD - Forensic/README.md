# Forensic

Digital forensics is about recovering and analysing evidence from an artifact
someone hands you — a memory image, a disk image, a packet capture, a suspicious
document. The goal is usually to answer "what happened / what was hidden here",
which maps cleanly onto the "forensics" and "stego" categories of most CTFs.

## Memory dump

A memory (RAM) dump is a snapshot of a machine's volatile memory. It can contain
running processes, open network connections, command history, cached
credentials and encryption keys — things that never touch the disk.

### Volatility

[Volatility](https://github.com/volatilityfoundation/volatility3) is one of the
best open-source frameworks for analysing RAM images from 32-/64-bit systems. It
supports Linux, Windows, macOS and Android.

> **Volatility 2 vs 3.** The commands below use **Volatility 2** syntax, which
> needs a `--profile` matching the exact OS build. **Volatility 3** (the current
> version) dropped `--profile` (it auto-detects) and renamed plugins with an
> OS prefix, e.g. `windows.pslist`, `windows.hashdump`, `windows.netscan`. Check
> which version you have with `vol.py --help` or `vol --help`.

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

## Steganography

Steganography hides data *inside* another file (often an image or audio file) so
that it looks ordinary. Useful tools:

```
exiftool <file>              Read all metadata (broader than the exif tool above)
binwalk <file>               Detect files embedded inside another file (see above)
steghide extract -sf <file>  Extract data hidden by steghide (often passphrase-protected)
zsteg <file.png>             Detect LSB-hidden data in PNG/BMP images
strings <file>               Sometimes the flag is just sitting there as text
```

* **StegSolve** (Java GUI) steps through colour planes and bit planes of an
  image to reveal hidden pixels: http://www.caesum.com/handbook/stego.htm
* For audio, inspect the **spectrogram** (e.g. in Audacity or Sonic Visualiser)
  — hidden text is sometimes drawn into the frequencies.
* Online, [Aperi'Solve](https://www.aperisolve.com/) runs several of these at
  once on an uploaded image.

## Device/Network analysis

### Network

```
tshark -2 -r <capture>.pcap -R "<wireshark_filter>" -T fields -e <field>        Export data from a PCAP file.
```

Package name: `sudo apt install tshark`

### Device

#### Keyboard

On Linux, keyboards appear as event devices under `/dev/input/eventX`, and each
keypress/release is a fixed-size `struct input_event`. A capture of that raw
stream (your own device, or one recovered as forensic evidence) can be replayed
offline to reconstruct what was typed.

The program [`./devices/keyboard/keylogger-reader.c`](devices/keyboard/keylogger-reader.c)
parses such a capture file and reconstructs the text, mapping keycodes to
characters (it ships with an **FR / AZERTY** layout — adjust the `keys[]` table
for other layouts). It handles Shift and AltGr and annotates special keys like
`[BACKSPACE]` and the arrow keys.

```sh
# Compile
gcc keylogger-reader.c -o keylogger-reader

# Parse a previously captured event stream into readable text
./keylogger-reader <capture_file> <output.txt>
```

> To produce a capture in the first place (on a device you own) you would read
> from the event device, e.g. `cat /dev/input/event3 > capture` — which needs
> root. Identify the right event number via `cat /proc/bus/input/devices` or
> `evtest`.

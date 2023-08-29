# Raspberry Pi Transfolio/rpfolio setup

The easiest way to transfer files onto an Atari Portfolio is by using the Smart Parallel Interface and the file transfer utility, with a Raspberry Pi acting as the file transfer source.
A Pi Zero running SSH will be more than sufficient for this. When correctly wired up it will look something like the following:

![Picture of wired up Pi Zero for rpfolio](/docs/graphics/images/rpfolio-pizero.jpeg)

The pin wiring/mappings are as follows:

```
+-------------+----------+
| Pi GPIO Pin | DB25 Pin |
+-------------+----------+
| GPIO4       | 3        |
| GND         | 25       |
| GPIO17      | 2        |
| GPIO27      | 12       |
| GPIO22      | 13       |
+-------------+----------+
```

![rpfolio Wiring diagram of Raspberry Pi Zero](/docs/graphics/images/pofo-rpfolio.svg)

To setup the Pi Zerp you will need to install Raspbian Pi Buster Lite on the Pi as later versions are missing the required wiringPi utilities
Once the Pi is setup and wifi/SSH etc configured, SSH in and use the following commands:

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git git-core
sudo apt install wiringpi=2.50
git clone https://github.com/skudi/transfolio.git
cd transfolio
make rpfolio
chmod +x rpfolio
sudo cp rpfolio /usr/local/bin/
```

This will install and make available the `rpfolio` command:

```
Syntax: rpfolio [-f] {-t|-r} SOURCE DEST 
  or    rpfolio -l PATTERN 

-t  Transmit file(s) to Portfolio.
    Wildcards are not directly supported but may be expanded
    by the shell to generate a list of source files.
-r  Receive file(s) from Portfolio.
    Wildcards in SOURCE are evaluated by the Portfolio.
    In a Unix like shell, quoting is required.
-l  List directory files on Portfolio matching PATTERN 
-f  Force overwriting an existing file 

Notes:
- SOURCE may be a single file or a list of files.
  In the latter case, DEST specifies a directory.
- The Portfolio must be in server mode when running this program!
```

To use this utility, from the Portfolio, enter the "Set up" menu (Atari + S keys) and select File transfer -> Server. The Portfolio screen should now say "Waiting for connection".
![Images of Atari Portfolio showing file transfer steps](/docs/graphics/images/pf-file-transfer.jpeg)


You can then begin to transfer files from the Pi using rpfolio with something like:

```
rpfolio -t -f SRC_FILE.EXE DST_FILE.EXE
```

This will copy the file `SRC_FILE.EXE` from the local filesystem on the Pi and store it on the Portfolio file system with the name `DST_FILE.EXE`


# Atari Portfolio PIN hacking program

<p align="center">
  <img src="/docs/graphics/images/portfolio-pin-program.jpeg" width="75%">
</p>

### Running the program
The executable generated is called `PIN_HACK.EXE` (available [here](https://github.com/plb500/T2-Portfolio-Hacker/releases/latest)) and can be run in three different ways, depending on which hardware you want to connect to.
- To connect to the Portfolio Serial Interface hardware, run:
```
PIN_HACK.EXE S
```
- To connect to the Portfolio Parallel Interface hardware (i.e. the Arduino running the code [here](hack_box)), run:
```
PIN_HACK.EXE P
```
- To not connect to any hardware and simply scroll down to a PIN you supply, run:
```
PIN_HACK.EXE <PIN Code>
```
e.g. to scroll to the PIN 9003, run:
```
PIN_HACK.EXE 9003
```

In the movie itself, sharp-eyed viewers will notice the name of the program John Connor types on screen is actually "pinid":

<p align="center">
  <img src="/docs/graphics/images/dos_batch_screenshot.png" width="75%">
</p>

This appears to be a DOS batch file - you can see that it shows "echo off" as the first line, which is a DOS command that stops the commands in the batch file from echoing to screen as they execute. Included in the [release](https://github.com/plb500/T2-Portfolio-Hacker/releases/latest) is a batch file called `pinid.bat` which can be edited to run any of the above commands and provide a screen-accurate emulation.

## Building the code
### Pre-requisites:
- DOSBox installed (https://www.dosbox.com/)
- A copy of Turbo-C (I am using version 2.01, downloaded from [here](https://winworldpc.com/product/borland-turbo-c/2x))

### Turbo-C setup
(NB: These instructions are for Mac OS. Modifying the DOSBox preferences is likely different on other systems)

- After installing and setting up DOSBox, set up your DOS development folder/drive as per the instructions [here](https://www.dosbox.com/wiki/DOSBox_and_Mac_OS_X).
  For the purposes of these instructions we will assume that I have mounted the folder `/Users/plb500/DOSDev` as drive C:
- Place your extracted Turbo C folder in `/Users/plb500/DOSDev` (e.g. `/Users/plb500/DOSDev/TURBO_C`)
- In a **terminal window**, navigate to the Turbo C folder
  ```
  cd /Users/plb500/DOSDev/TURBO_C
  ```
- Disk swapping in DOSBox on Mac OS seems to be a bit of a nightmare, so we are going to create a temporary disk image file we will mount inside of DOSBox and simply copy the different floppy images to that file when it's time to swap disks (don't worry, it'll become clear later). For now, copy the initial install disk to the temporary image
  ```
  cp Install.img disk.img
  ```
- Launch DOSBox
- Inside **DOSBox**, navigate to the Turbo C directory and mount the temporary drive file (masquerading as the Turbo C install disk) as drive A:
  ```
  C:
  cd \TURBO_C
  imgmount a: -t floppy disk.img
  ```
- Start the installer
  ```
  A:
  install
  ```
- Run through the installer, picking drive A as the source drive, and selecting the option "Install Turbo C on a Hard Drive"
- Pick the directories you want to install everything to (e.g. `C:\TC`)and begin the installation process by selecting "Start Installation"
- When prompted to insert the "COMPILER/UTILITIES" disk, switch to the **terminal window** and copy the compiler disk image to the temporary disk image file
  ```
  cd /Users/plb500/DOSDev/TURBO_C
  cp Compiler.img disk.img 
  ```
- Switch back to **DOSBox** and hit ENTER to continue the install process
- When prompted to insert the "HEADER FILES/LIBRARIES" disk, switch to the **terminal window** and copy the header disk image to the temporary disk image file:
  ```
  cd /Users/plb500/DOSDev/TURBO_C
  cp Header.img disk.img
  ```
- Switch back to **DOSBox** and hit ENTER to continue the install process
- The install should complete successfully and you should have Turbo C installed somewhere on your DOS development drive/folder. As per the instructions above, ensure that the Turbo C folder (e.g. `C:\TC`) is added to your path inside of your preferences file, e.g. by adding the line:
```
PATH=Z:\;C:\TC
```
- At this point, you should be able to build the Portfolio code using the `MAKE.BAT` batch file included in this repo


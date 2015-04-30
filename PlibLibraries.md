## Introduction ##

I dug around looking for answers to some of the questions regarding how libraries are linked into projects. I've also include some general information just in case it would save anyone some time.


---

## Includes ##

The compiler looks for includes files in
> ../Microchip/MPLAB C32 Suite/pic32mx/includes


---


## Libraries ##

The library source is located at
> ../Microchip/MPLAB C32 Suite/pic32-libs/

The library peripheral source is located under:
> ../Microchip/MPLAB C32 Suite/pic32-libs/peripheral

I looked at the make files for these and here are some observations.
  * The makefiles were compiled under linux/unix and use extensive use of bash.
  * All the libraries in pic32-libs are linked into chip specific .a files located at (where the linker finds them):
> > ../Microchip/MPLAB C32 Suite/pic32mx/lib
  * The pic32mx/includes folder is identical to the same in pic32-libs (this is the 'source' it was created from)

## Linking ##


(From the GNU make documentation) The .a files are archive files that are staticly linked into the code. Using the GNU tools on my linux box I used the GNU archiver 'ar' to view the contents of the archive. This is the same tool microchip used to create their archive (at list from viewing the makefile), we can (using the right flags) reproduce the same effect, by making each code unit into separate files (to create unique .o files) and linking this into our own archive.

From the MPLIB Documentation

Microchip provides a MPLIB program to manage libraries see the MPLIB help topics in MPLAB. It provides an IDE and command line interface to managing libraries.

MPLIB note on how objects are linked.
  * Libraries help keep code small. Since a linker only uses the required object files contained in a library, not all object files which are contained in the library necessarily wind up in the linker's output module.

I have confirmed that some of the libraries .c code is separated in function-per-file chunks as we discussed last Tuesday. A few observations:

  * Can -  is devided into files for functionality
  * CANConfiguration, CANEventFunctions, CANFilterFuncations....
  * SPI - Is divided into many files with only a few functions each
  * I2C - is divided into functions-per-file
  * Uart - is a single file

We should use the same reasoning in our own development to determine when the reduced size merits the extra work to divide the source into discrete files vs when code will always be used together and will always be linked anyway.
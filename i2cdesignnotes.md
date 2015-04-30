#i2c design notes

# Introduction #

Note: formatting for this wiki is different then I'm use to. I'm still working on it, sorry for the readability.

The Microchip I2C PLIB comes in two flavors. The I2C legacy  (V1.0 depreciated) and version 2.0.

See

  * Microchip PIC32MX Peripheral Library
```
 Microchip/MPLAB C32 Suite/doc/Microchip-PIC32MX-I2C-Peripheral Library```

  * Microchip PIC32MX I2C Peripheral Library
```
 Microchip/MPLAB C32 Suite/doc/pic32-lib-help/i2C-Plib-Help```

  * I2c Specification
```
[http://www.i2c-bus.org/ http://www.i2c-bus.org/]```

# Legacy I2C #

The Legacy I2C design is depreciated. Microchip suggests that projects do not use this code, it is only provided for legacy applications.

To ensure legacy code is not used the following should be defined before calling any plib headers.
<pre>
#define _PLIB_DISABLE_LEGACY<br>
</pre>

# I2C V2.0 #

Microchip suggests that to use any plib's that the plib.h header be included. This header will include the correct includes for the microprocessor defined in the MPLAB IDE. After including plib.h any plib function can be called. The linker will only link the object files that are actually needed.

From the i2C-Plib-Help Introduction
```
This library provides a low-level abstraction of the Inter-Integrated Circuit (I2C) module on Microchip PIC32MX family microcontrollers with a convenient C language interface. It can be used to simplify low-level access to the module without the necessity of interacting directly with the module's registers, thus hiding differences from one microcontroller variant to another.

Please refer to the I2C-Bus Specification (v2.1), available from Philips(1) Semiconductor for complete details on the operation and requirements for the I2C bus.

```
# I2C interrupts #

The suggested way to poll the I2C buss is via interrupts. This is where I'm looking into now.

# I2C Special Considerations #

The I2C1 on the 32mx4 contains two devices (IC2 and IC3) on the buss and has default pull up resistors (only one device on an I2C buss should have this).

  * IC2 is a 24LC256 (32Kbyte serial EEPROM device) (Address 0x50). This device uses a bitmask to address the entire device space this limits the number of devices that can be on this port.
  * IC3 is a MCP4725 (single channel, 12-bit, serial digital to analog converter) (Address 0x60).

The I2C2 port does not contain any devices and has optional pull up resistor jumpers.

# Next Steps #

Stuff that I think should happen in no order.

## interrupt handling ##
I would like to better understand how the interrupt handling works so I can write the interuptservicehandler's for the device. I looked into the header files (for this i'ts int\_3xx\_4xx.h ) and there are some extra options that the example (brief example) code doesn't cover. I would then want to write basic interrupt handlers.

## I2C1 ##

The I2C1 already contains a number of devices. It would be useful to supply code to interact with these devices for the user. I haven't yet found out if they can be disabled. If not the total address space of I2C1 will be reduced by how IC2 interacts with the master.
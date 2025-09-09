# DEMOCLE
# DEclarative Multi-agent Open C-Language Engine

## Installation

Clone the repository by using:

>     $ git clone https://github.com/UniCT-ARSLab/DEMOCLE

## Compilation for Linux
Enter the DEMOCLE directory, then:

>     $ mkdir build
>     $ cd build
>     $ cmake ..
>     $ make

 The platform will be compiled also with all examples provided in the **example** folder.

## Compilation for MCUs
Currently DEMOCLE has been tested for the following MCUs:

 - ESP32 WROOM
 - Arduino Nano ESP32
 - ESP32C3
 - STM32F401RE

It can be compiled using the VSCode/PlatformIO tool.
The platformio.ini file contains the environment definitions for the boards/MCUs tested till now.

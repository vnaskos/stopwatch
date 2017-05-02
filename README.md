# Stopwatch
This project is an implementation of a stopwatch on an Atmega16 microcontroller, written in embedded C.

## Simulation
![stopwatch-simulation](https://cloud.githubusercontent.com/assets/6230644/25638882/8f7c9a04-2f92-11e7-9b98-b3b3ba48b78a.gif)

## How it work
One push button is responsible to start and stop the time. The time is counted on timer0 of the atmega16 microcontroller. Time is displayed live on a 4×7 segment, using BCD code and a driver chip to select and enable the appropriate digit/segment. The second button, captures and displays the current time on the LCD screen, with the help of lcd.h library. Finally, the third button clears the screen, but only if the timer is stopped.

## Parts
The stopwatch consists of:
* atmega16 microcontroller
* 4×7 segment display
* LCD screen
* 7 segment BCD display driver
* 3 push buttons

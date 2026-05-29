# Handheld Gaming Device

A PIC24FJ64GA002 handheld game console with a 128x128 OLED, dual analog
joysticks, and four built-in games. Firmware in C, hardware in KiCad.

## Games

- **Pong** - two-player, first to 5
- **Ackanoid** - brick breaker, 3 levels, 3 lives
- **Maze** - 2.5D raycaster on a randomly generated maze
- **Dinosaur Runner** - endless side-scroller, jump with the blue button

## Controls

- **Joysticks** - menu navigation and in-game movement
- **Blue button** - select / jump
- **Red button** - back
- **Joystick click + Red (held)** - exit a game to the main menu

## Build

MPLAB X IDE + XC16 compiler. Open the `.X` folder as a project, build, and
flash with a PICkit (or equivalent) via the ICSP header.

## Hardware

KiCad schematic and PCB live in `hardware/`. Built around a PIC24FJ64GA002
(28-pin DIP), an SH1107-class 128x128 OLED on SPI1, two analog joysticks,
two pushbuttons, and an NPN-driven piezo speaker. Pin assignments are in
[`Setup.c`](Setup.c).

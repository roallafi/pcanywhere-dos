# PCAnywhere DOS Clone

This repository contains a small educational DOS serial terminal prototype inspired by PCAnywhere v4.

It is designed for:
- MS-DOS / DOSBox
- Borland C++ 4.0 / Turbo C++ 4.0
- RS-232 COM serial communication

## Project goal
The program is intentionally small and educational. It demonstrates:
- COM port configuration
- custom packet protocol
- keyboard capture via DOS interrupt hooks
- screen capture from video memory
- simple server/client interaction over serial port

## Important note
This is not a full PCAnywhere replacement. It is a testable baseline for learning how serial remote control works in DOS.

## Files
- `pcanywhere.cpp` - main program and menu
- `serial.h` / `serial.cpp` - COM port driver
- `protocol.h` / `protocol.cpp` - custom packet protocol
- `keyboard.h` / `keyboard.cpp` - keyboard capture
- `screen.h` / `screen.cpp` - screen capture and update
- `display.h` / `display.cpp` - basic DOS display helper
- `Makefile` - Borland build file
- `build.bat` - DOSBox-friendly compilation helper
- `dosbox_pcanywhere.conf` - sample DOSBox config

## Build instructions for DOSBox

1. Install DOSBox.
2. Put this project inside a DOSBox-mounted folder, for example:
   - `C:\work\pcanywhere-dos`
3. Launch DOSBox and mount the folder:

```bat
mount c c:\work\pcanywhere-dos
c:
```

4. Compile using Borland C++ 4.0 or Turbo C++ 4.0:

```bat
build
```

If you are using a Borland toolchain directly:

```bat
bcc -O -c pcanywhere.cpp
bcc -O -c serial.cpp
bcc -O -c protocol.cpp
bcc -O -c keyboard.cpp
bcc -O -c screen.cpp
bcc -O -c display.cpp
bcc -e pcanywhere.exe pcanywhere.obj serial.obj protocol.obj keyboard.obj screen.obj display.obj
```

5. Run:

```bat
pcanywhere.exe
```

## Runtime notes
- Use the same baud rate on both connected machines.
- Use the same COM port selection on both ends.
- Cable must be proper RS-232 null-modem or equivalent.
- For simpler testing, use two DOSBox sessions connected by a null-modem emulation or a real serial link.

## Using DOSBox serial emulation
If you want to test between two DOSBox sessions, use a null-modem pair and configure the serial ports similarly. In practice, the simplest path is:
- one DOSBox session = server
- second DOSBox session = client
- connect both to the same virtual serial device or host serial bridge

## Current status
This is a DOSBox-ready educational prototype and should be treated as a starting point for learning and experimentation.

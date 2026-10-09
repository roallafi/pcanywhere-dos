# PCAnywhere DOS Clone

⚠️ **WARNING - EDUCATIONAL USE ONLY**

This is an educational prototype and **not a production-ready application**.

**Key limitations:**
- NOT tested on real MS-DOS hardware
- NOT fully compatible with all Borland C++ 4.0 configurations
- Serial communication may require manual tuning for your hardware
- Keyboard and screen capture hooks are basic implementations
- Use only for learning purposes in controlled environments (DOSBox, VM)
- NOT recommended for actual remote desktop use

---

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

## Changes from the initial keyboard version

### keyboard.h / keyboard.cpp
The keyboard capture implementation was updated to be more compatible with Borland C++ 4.0 and safer for a DOS interrupt-based design.

**What changed:**
1. **Added `old_handler` member**
   - The updated version stores the original interrupt handler before installing a custom one.
   - This is important because DOS programs must restore previous interrupt vectors when they are done.

2. **Improved `install()` behavior**
   - Initial version: set the vector directly with `setvect(KEYBOARD_INT, keyboard_isr)`.
   - Updated version: first saves the old vector with `old_handler = getvect(KEYBOARD_INT)`, then replaces it.

3. **Improved `uninstall()` behavior**
   - Initial version: did not preserve the original handler correctly.
   - Updated version: restores the saved `old_handler`, which prevents leaving the system on a custom ISR after program exit.

4. **Added explicit far interrupt declaration**
   - `static void interrupt far keyboard_isr(...)`
   - This matches the way DOS interrupt handlers are declared in Borland C++ and avoids incorrect far/near call assumptions.

5. **Improved safety around re-entry and state**
   - The updated implementation tracks whether the handler is already installed and avoids multiple installations.

### Why this matters
Without saving and restoring the old interrupt handler:
- the keyboard interrupt chain can be broken
- the system may become unstable
- subsequent runs may hang or behave unpredictably
- DOS may stop handling normal keyboard input correctly

### Example difference
- Initial approach: custom ISR installed, but previous vector not preserved
- Updated approach: save old vector, replace it, then restore old vector on exit

This makes the keyboard hook more like a proper DOS TSR-style patch instead of a dangerous one-time override.

## Current status
This is a DOSBox-ready educational prototype and should be treated as a starting point for learning and experimentation.

**For testing:** Use a controlled DOSBox environment with two separate instances or a proper DOS VM setup.

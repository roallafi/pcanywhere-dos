# PCAnywhere DOS Clone

Educational MS-DOS serial remote-control prototype modeled after PCAnywhere v4 style, designed for Borland C++ 4.0.

## What is included
- Serial COM port communication
- Simple custom packet protocol with checksum
- Keyboard capture support using DOS interrupt hooks
- Screen capture via direct video memory access
- Client/server menu for serial connection testing
- Borland C++ 4.0 makefile

## Build

Use Borland C++ 4.0 in DOS or a DOS-compatible environment:

```bat
make
```

Then run:

```bat
pcanywhere.exe
```

## Notes
This project is intentionally simple and educational. It is not a full PCAnywhere replacement, and it is intended to be tested in a DOS environment such as DOSBox or a real MS-DOS machine.

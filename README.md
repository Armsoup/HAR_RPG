# HAR_RPG

**HAR_RPG** is a portable console RPG game built on the **ArmsoupHAREngine** — a lightweight C++ game framework powered by WinAPI.

> *"H — who knows, AR — ARSLANIUS."*

The project is split into two parts:
- **ArmsoupHAREngine** — the core framework (console handling, files, saves, colors)
- **HAR_RPG** — the game itself (player, hunger, monsters, inventory, open world)

Everything runs directly from a single `.exe`, saves progress to `save.txt`, and fits in under 1 MB.
> **Starting with [version HAR 6.3](https://github.com/Armsoup/HAR_RPG/releases/tag/RPG_GAME6.3), the compilation and building of .exe and .cpp files are performed in the "VisualStudio 2022 Community" environment.**

---

## Features

- **Open world** — move freely in 2D (and 3D on HAR3D) coordinates (X, Y)
- **Hunger system** — 0–40 scale, every step costs food
- **Monster encounters** — random fights that block your path
- **Inventory** — collect and use food at any time
- **Save/Load** — progress persists in `save.txt`
- **Console UI** — pure WinAPI, no graphical dependencies
- **Modular engine** — you can fork the engine and build your own game

---

## Controls

| Key | Action |
|-----|--------|
| `W` | Move forward (costs 1 hunger) |
| `S` | Move backward (costs 1 hunger, if possible) |
| `A` | Move left (costs 1 hunger) |
| `D` | Move right (costs 1 hunger) |
| `H` | Eat from inventory (restores 1 hunger) |
| `G` | Fight the monster (if present) |

---

##  Installation

1. Download the latest `HAR.exe` and `HAR3D.exe` from [Releases](https://github.com/Armsoup/HAR_RPG/releases)
2. Run the `.exe`
3. Enter your name and start surviving

No installation, no dependencies, no registry entries.

---

## For Developers

The engine and game are fully open-source under AGPL-3.0.

Project structure:

· ArmsoupHAREngine.hpp — engine implementation

· HAR.cpp — game logic and main loop

· HAR3D.cpp — game logic and main loop on 3D

To build with MSVC (x64 Native Tools Command Prompt):

```
cl /EHsc /Fe:HAR.exe HAR.cpp
cl /EHsc /Fe:HAR3D.exe HAR3D.cpp
```

The engine is designed to be reusable — you can drop it into any console project and get instant file/save/color support.

---

License

HAR_RPG
Copyright © Armsoup 2026

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Full license text is available in the LICENSE file.

---

Author

Armsoup — also the creator of:

· [ARSLANIUS](https://github.com/Armsoup/ARSLANIUS_C-Plus_Plus) — console operating system simulator

· [Useless TEMP Cleaner](https://github.com/Armsoup/Useless-TEMP-cleaning-program) — background temp cleaner

"I write code because I enjoy it. And I recommend you do the same."

---

Support

If you like the game — leave a star

If you find a bug — open an [Issue](https://github.com/Armsoup/HAR_RPG/issues)

If you want to improve it — [send a Pull Request](https://github.com/Armsoup/HAR_RPG/pulls)

---

© Armsoup 2026

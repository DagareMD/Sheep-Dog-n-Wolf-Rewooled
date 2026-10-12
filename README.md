<p align="center">
    <img src="https://github.com/SDW-Community/Sheep-Dog-n-Wolf-Rewooled/blob/main/res/banner.png?raw=true" style="width: 512px;" />
</p>

<h1 align="center">Sheep, Dog 'n' Wolf: Rewooled</h1>

<h3 align="center">An open-source multiplatform implementation of Sheep, Dog 'n' Wolf PC </h3>


<div align="center">


[![github.com](https://img.shields.io/github/v/release/SDW-Community/Sheep-Dog-n-Wolf-Rewooled.svg?color=green)](https://github.com/SheepStealer2001/Sheep-Dog-n-Wolf-Rewooled/releases/latest)

</div>

---
### How to use
- **You need to provide your own files from the original game version, this project does not include any assets**
- **Although it is supported, PSX files are still not fully tested and there are quirks to work out **
- On the launcher you simply browse for your original game folder
### Features

- **Multiplatform:**  All Windows only APIs replaced via SDL3
- **Multiple Renderers** Support for Vulkan, DX12 and Metal
- **Modern Controllers:** Support for modern controllers via SDL3
- **Scaled Textures:** Support for swapping texture for arbitrarily sized PNGs

### For Mac users

As the releases are unsigned, you will need to run
`xattr -r -d com.apple.quarantine Rewooled.app`

to prevent your OS from blocking the execution

### Building

It builds for **Windows (MSVC, x64)**, **macOS (Apple Clang, arm64)** and **Linux (Clang, x86_64 or arm64)**.

For Linux users, it is also possible to cross-compile for Windows 64-bit using MinGW-w64 (Clang) via the provided `build_win64.sh` script. This requires the `mingw-w64-gcc` package (on Arch Linux). Note that the resulting `.exe` requires the MinGW runtime DLLs (e.g., `libstdc++-6.dll`, `libgcc_s_seh-1.dll`, `libwinpthread-1.dll`) to be present in the game folder to run.

You need your own copy of the game: its data is read at run time and is not part of this repository.

**How it was made:** the decompilation was done with Claude and Codex coordinating via an MCP mailbox server - reading the machine code, naming, writing the source. In-game testing and technical knowledge about the game's inner workings came from the Sheep Raider community.



## Credits
- **lu9** For making an icon for this project
- **uhwot**, for [sdw_re_stuff](https://github.com/uhwot/sdw_re_stuff) (MIT): the research into the game's asset formats (WAR, DAV, SND and others), with an exporter and ImHex patterns.
- **The whole Sheep Raider Discord community**, for helping crack this game open.


## Licence

TBD

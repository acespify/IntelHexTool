# Intel HEX Tool

A graphical utility for parsing, analyzing, and disassembling Intel HEX object files, built with C++, SDL2, and Dear ImGui.
*Note: You should create a 'docs' folder and place a screenshot of the running application there.*

---

## About The Project

This tool provides a simple and intuitive interface for developers and hobbyists working with firmware for microcontrollers and older CPU architectures. It allows you to load an Intel HEX file, view its memory layout, and see a full disassembly of the machine code, complete with automatically generated labels for code branches.

---

## Features

* **Intel HEX Parsing**: Loads and validates standard Intel HEX files.
* **Memory Viewer**: Displays the complete memory map in a classic hex editor format.
* **Intel 8080 Disassembler**: Translates the raw machine code into human-readable Intel 8080 assembly instructions.
* **Symbol Analysis**: Automatically detects `JMP` and `CALL` targets to generate and display code labels (e.g., `L401A:`).
* **Sparse Memory Handling**: Intelligently skips empty memory regions in the disassembly view, preventing long lists of `NOP`s.
* **Save Disassembly**: Exports the full disassembly listing, with labels, to a `.txt` or `.asm` file.

---

## Technology Stack

* **C++17**: The core application language.
* **SDL2**: For creating the window and handling rendering, input, and events.
* **Dear ImGui**: For the immediate-mode graphical user interface.
* **Makefile**: For building the project on Windows with a MinGW-w64 toolchain (UCRT64).

---

## Getting Started

Follow these instructions to get a local copy up and running for development.

### Prerequisites

You will need a working C++ development environment on Windows. This project is built using:
* **MSYS2 with UCRT64**: Download from [msys2.org](https://www.msys2.org/).
* **MinGW-w64 UCRT64 toolchain**: Install within MSYS2 using `pacman -S mingw-w64-ucrt-x86_64-toolchain`.

### Installation & Setup

1.  **Clone the repository:**
    ```sh
    git clone [https://github.com/your_username/IntelHexTool.git](https://github.com/your_username/IntelHexTool.git)
    cd IntelHexTool
    ```

2.  **Set up Libraries:** This project uses a local library structure. You will need to download the following and place them in the correct folders:

    * **SDL2:** Download the MinGW development library.
        * Place all `.h` files into `includes/SDL2/`.
        * Place all `.a` and `.dll` files into `lib/`.

    * **Dear ImGui:** Download the latest version.
        * Place all `.h` files into `includes/imgui/`.
        * Place all `.cpp` files (including `imgui_impl_sdl2.cpp` and `imgui_impl_sdlrenderer2.cpp`) into the `imgui/` folder.

    * **ImGuiFileDialog:** Download the latest version.
        * Place `ImGuiFileDialog.h` into the `includes/` folder.
        * Place `ImGuiFileDialog.cpp` into the `src/` folder.

    Your final project structure should look like this:
    ```
    IntelHexTool/
    ├── imgui/         (ImGui .cpp files)
    ├── includes/
    │   ├── imgui/     (ImGui .h files)
    │   └── SDL2/      (SDL2 .h files)
    ├── lib/           (SDL2 .a and .dll files)
    ├── src/           (Your .cpp files)
    └── Makefile
    ```

### Building

The included `Makefile` handles both debug and release builds. Open a **UCRT64 terminal** in the project root.

* **To create a debug build (with debugging symbols):**
    ```sh
    make debug
    ```
    The executable will be located at `build/debug/IntelHexTool.exe`.

* **To create an optimized release build:**
    ```sh
    make release
    ```
    The executable will be located at `build/release/IntelHexTool.exe`.

* **To clean all build files:**
    ```sh
    make clean
    ```

---

## Usage

After building, run the executable.
1.  Click **"Open Hex File"** to load a `.hex` or `.txt` file.
2.  The "Intel HEX Tool" window will show the raw parsed records.
3.  The "Memory Viewer" window will show the complete memory layout.
4.  The "8080 Disassembly" window will show the resulting assembly code.
5.  Click **"Save Disassembly"** in the disassembly window to export the listing to a file.

---

## Roadmap

This is an ongoing project. Future planned features include:
* [ ] Full disassembly for the Intel 8085 and Z80 CPUs.
* [ ] Support for 16-bit (Extended Segment) and 32-bit (Extended Linear) HEX file formats.
* [ ] A memory editor to modify values directly.
* [ ] An "assembler" feature to write assembly and generate a new HEX file.
* [ ] Generation of raw binary files for PROM programmers.

---

## License

Distributed under the MIT License. See `LICENSE` file for more information.

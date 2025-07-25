# Hack Virtual Machine Translator in C

A VM translator for the Hack platform, written in C. This project is part of the Nand2Tetris course and translates high-level VM code into Hack assembly language, serving as the backend for the Jack compiler.

---

## About The Project

This project implements a complete VM translator that handles the full VM language specification, including stack operations, program flow, function calls, and memory segment management. The architecture features a modular design with separate components for parsing, translation, and code generation.

### Features

- Full VM language support (stack arithmetic, memory access, program flow, function calls)
- Handles both single VM files and multi-file VM programs
- Implements the standard VM memory segments (local, argument, this, that, static, etc.)
- Supports function calls with proper stack frame management
- Implements recursive function calls and return handling
- Generates efficient Hack assembly with optimized instruction sequences
- Comprehensive error handling and validation
- Makefile-based build system with debug/release configurations

### Built With

- [C](<https://en.wikipedia.org/wiki/C_(programming_language)>)
- [GNU Make](https://www.gnu.org/software/make/)
- [Clang](https://clang.llvm.org/)
- [Bear](https://github.com/rizsotto/Bear)

---

## Getting Started

To get a local copy up and running, follow these steps.

### Prerequisites

A C toolchain (`clang` is recommended), `make`, and `bear` must be installed.

#### Linux

- **On Arch Linux:**
  ```sh
  sudo pacman -S clang base-devel make bear
  ```
- **On Debian/Ubuntu:**
  ```sh
  sudo apt-get install clang build-essential make bear
  ```

### Building the Project

1.  **Clone the repository:**

    ```sh
    git clone https://github.com/francis1009/nand2tetris-vm-translator.git
    cd nand2tetris-vm-translator
    ```

2.  **Generate `compile_commands.json`:**
    This step configures the editor's language server (e.g., `clangd`). It only needs to be run once, or whenever source files are added or removed.

    ```sh
    bear -- make debug
    ```

3.  **Build the translator:**
    This creates the executable at `bin/vm_translator_debug` or `bin/vm_translator`.

    ```sh
    # For a debug build with extra checks
    make debug

    # For an optimized release build
    make release
    ```

---

## Usage

The VM translator processes either single VM files or directories containing multiple VM files:

1.  Place your VM files (e.g., `SimpleAdd.vm`) or directories (e.g., `StaticsTest/`) inside the `vm/` directory.

2.  Run the translator from the project root, specifying the file or directory name with the FILE variable:

    ```sh
    # Process a single VM file
    make run FILE=SimpleAdd.vm

    # Process a directory of VM files
    make run FILE=StaticsTest
    ```

3.  The generated Hack assembly will be written to a `.asm` file with the same name as the input file or directory in the project's root directory:
    - For `SimpleAdd.vm` → `SimpleAdd.asm`
    - For `StaticsTest/` → `StaticsTest.asm`

---

## Development

This project uses `clang-format` to maintain consistent code style. The rules are defined in the `.clang-format` file. If you have a "format-on-save" feature configured in your editor, it will be applied automatically.

---

## Acknowledgments

This project is an implementation of the VM translator described in the Nand2Tetris course. The architecture and specifications are based on the principles taught in the course.

- **The Nand2Tetris Website ([nand2tetris.org](https://www.nand2tetris.org/))**: The official resource for all course materials, software tools, and forums.

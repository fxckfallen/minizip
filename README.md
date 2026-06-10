# MiniZip Archiver

A fast, lossless command-line file compression utility written from scratch in pure C++. 

This project implements the classic **Huffman Coding algorithm** to compress and decompress files. It reads raw binary data, builds a frequency-based binary tree, and packs the resulting variable-length codes bit-by-bit into a compressed archive.

## Features & Under the Hood

This project was built as an engineering challenge to deeply understand memory management and bitwise operations in C++. 

* **Zero Dependencies:** Written in standard C++17. No third-party libraries (like Boost or zlib) are used.
* **Bitwise Magic:** Features a custom bit-buffer implementation to pack and unpack individual bits (`0`s and `1`s) into 8-bit bytes using bitwise shifts (`<<`, `>>`) and masks (`|`, `&`).
* **Binary Safe:** Uses `std::ifstream` and `std::ofstream` in strict binary mode, meaning it can compress *any* file type (Text, Images, Executables), not just plain text.
* **Graph & Memory Management:** Implements a custom Node structure and utilizes `std::priority_queue` with a custom comparator to efficiently build the Huffman tree in the heap.

## Build Instructions

The project uses **CMake** as its build system, making it cross-platform (Windows, macOS, Linux).

1. Clone the repository:
   ```bash
   git clone https://github.com/fxckfallen/minizip.git
   cd minizip
   ```

2. Create a build directory and generate the build files:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Compile the project:
   ```bash
   cmake --build .
   ```

## Usage

The compiled executable (named `app` or `app.exe` on Windows) takes three arguments: the mode (`compress` or `decompress`), the input file, and the output file.

**To compress a file:**
```bash
./app compress input.txt archive.mz
```
*(This will analyze `input.txt`, generate a Huffman tree, and write the compressed bits along with the header to `archive.mz`)*.

**To decompress a file:**
```bash
./app decompress archive.mz restored_input.txt
```
*(This will read the header from `archive.mz`, rebuild the exact same Huffman tree, decode the bitstream, and restore the file byte-by-byte).*

## What I Learned
- Deepened knowledge of standard C++ memory management and pointers.
- Mastered bitwise operations for low-level data manipulation.
- Learned to separate C++ architecture cleanly into headers (`.h`) and implementations (`.cpp`).
- Configured out-of-source builds using CMake.

---

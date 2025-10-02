# expOS

An experimental operating system for ARMv8-A architecture, written in C and Assembly.

## Building

### Prerequisites
- CMake (version 3.20 or higher)
- QEMU (for running the OS in a virtual environment)

### Downloading the toolchain
```bash
source ./scripts/toolchain.sh
```

### Building the project
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Running the OS in QEMU
```bash
cmake --build . --target run
```

## Project Structure

- `src/kernel`: Contains the kernel source code and linker script.
- `src/shared`: Contains shared utilities and libraries.
- `src/logging`: Contains logging utilities.

## Submitting Code

Please use clang-format for code formatting. A `.clang-format` file is provided in the root directory.
You can format your code using the following command:
```bash
clang-format -i src/**/*.c src/**/*.h
```

## License

GPL-3.0-Only License. See the LICENSE.md file for more details.

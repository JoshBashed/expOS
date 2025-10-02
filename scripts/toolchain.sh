#!/bin/bash

# Check if the script is being sourced
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "This script must be sourced, not executed."
    exit 1
fi

# Detect shell
if [ -n "$BASH_VERSION" ]; then
    SHELL_NAME="bash"
elif [ -n "$ZSH_VERSION" ]; then
    SHELL_NAME="zsh"
fi

TOOLCHAIN_AARCH64_NONE_ELF_DARWIN_ARM64=https://developer.arm.com/-/media/Files/downloads/gnu/14.3.rel1/binrel/arm-gnu-toolchain-14.3.rel1-darwin-arm64-aarch64-none-elf.tar.xz
TOOLCHAIN_AARCH64_NONE_ELF_DARWIN_ARM64_SHA256=8636ad0e42acab8227974c8180916b114040f5c77f8b7f5480fe58af4911d484
TOOLCHAIN_AARCH64_NONE_ELF_LINUX_X86_64=https://developer.arm.com/-/media/Files/downloads/gnu/14.3.rel1/binrel/arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-elf.tar.xz
TOOLCHAIN_AARCH64_NONE_ELF_LINUX_X86_64_SHA256=ebaf2d47f2e7f7b645864c5c8cf839e526daed83a2e675a3525d03f5ba3d2be9
TOOLCHAIN_AARCH64_NONE_ELF_LINUX_ARM64=https://developer.arm.com/-/media/Files/downloads/gnu/14.3.rel1/binrel/arm-gnu-toolchain-14.3.rel1-aarch64-aarch64-none-elf.tar.xz
TOOLCHAIN_AARCH64_NONE_ELF_LINUX_ARM64_SHA256=1bbbd25746234c60dd0a00d891b24a6b9848a25ed2b13d95e8d4c19cd78035e3

# Set default toolchain
TOOLCHAIN="AARCH64_NONE_ELF"

print() {
    echo "$@" >&2
}

# Detect OS
OS="UNKNOWN"
UNAME_S=$(uname -s)
if [ "$UNAME_S" = "Darwin" ]; then
    OS="DARWIN"
elif [ "$UNAME_S" = "Linux" ]; then
    OS="LINUX"
fi

# Detect Architecture
OS_ARCH="UNKNOWN"
UNAME_M=$(uname -m)
if [ "$UNAME_M" = "arm64" ]; then
    OS_ARCH="ARM64"
fi elif [ "$UNAME_M" = "x86_64" ]; then
    OS_ARCH="X86_64"
fi elif [ "$UNAME_M" = "aarch64" ]; then
    OS_ARCH="ARM64"
fi

# Ensure toolchain is supported
if [ "$OS" = "UNKNOWN" ] || [ "$OS_ARCH" = "UNKNOWN" ]; then
    print "Unsupported OS or architecture: $UNAME_S $UNAME_M"
    exit 1
fi

TOOLCHAIN_VAR="TOOLCHAIN_${TOOLCHAIN}_${OS}_${OS_ARCH}"
TOOLCHAIN_SHA256_VAR="${TOOLCHAIN_VAR}_SHA256"

# Check both variables are set
if [ "$SHELL_NAME" = "bash" ]; then
    TOOLCHAIN_URL="${!TOOLCHAIN_VAR}"
    TOOLCHAIN_SHA256="${!TOOLCHAIN_SHA256_VAR}"
elif [ "$SHELL_NAME" = "zsh" ]; then
    TOOLCHAIN_URL=$(eval echo "\$$TOOLCHAIN_VAR")
    TOOLCHAIN_SHA256=$(eval echo "\$$TOOLCHAIN_SHA256_VAR")

    if [ -z "$TOOLCHAIN_URL" ] || [ -z "$TOOLCHAIN_SHA256" ]; then
        print "Toolchain $TOOLCHAIN is not supported on $OS $OS_ARCH"
        return 1 2>/dev/null || exit 1
    fi
fi

# Download locations
ORIGINAL_DIR=$(pwd)
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
CACHE_DIR="$SCRIPT_DIR/../cache"
TOOLCHAIN_DIR="$CACHE_DIR/toolchain-$TOOLCHAIN-$OS-$OS_ARCH"

mkdir -p "$CACHE_DIR"
cd "$CACHE_DIR" || exit 1

# Download and extract toolchain if not already done
print "Checking for toolchain..."
if [ ! -d "$TOOLCHAIN_DIR" ]; then
    print "Downloading toolchain..."
    TOOLCHAIN_ARCHIVE="$TOOLCHAIN_DIR.tar.xz"
    curl -L -o "$TOOLCHAIN_ARCHIVE" "$TOOLCHAIN_URL"
    echo "$TOOLCHAIN_SHA256  $TOOLCHAIN_ARCHIVE" | shasum -a 256 -c -
    mkdir -p "$TOOLCHAIN_DIR"
    tar -xf "$TOOLCHAIN_ARCHIVE" --strip-components=1 -C "$TOOLCHAIN_DIR"
    rm "$TOOLCHAIN_ARCHIVE"
else
    print "Toolchain already downloaded."
fi

# Export toolchain to PATH
print "Using toolchain at $TOOLCHAIN_DIR"

# Export to PATH if not already present
if [[ ":$PATH:" != *":$TOOLCHAIN_DIR/bin:"* ]]; then
    export PATH="$TOOLCHAIN_DIR/bin:$PATH"
fi

# Return to original directory
cd "$ORIGINAL_DIR" || exit 1

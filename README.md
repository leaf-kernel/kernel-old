# Leaf
> [!NOTE]
> This is the old repo checkout the new one [here](https://github.com/leaf-kernel/os)

A hobby kernel for nerds. Pre built ISO's [here](https://nightly.link/leaf-kernel/kernel/workflows/build/main/Leaf-release.zip)

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/leaf-kernel/kernel/.github%2Fworkflows%2Fbuild.yml?label=Daily%20ISO%20Build%20(x86_64-limine))

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/leaf-kernel/kernel?label=Code%20Size%20(bytes))

Git Mirror available [here](https://git.leafkern.xyz/leaf)

Discord server available [here](https://discord.gg/mv9eePT7zV)

## Building Instructions (Local)

To build a bootable image, follow these steps:

1. **Configure the Kernel:**

   Use the `./env/configure` script to set up the kernel build environment.

   ```bash
   ./env/configure <target> <bootloader> <toolchain>
   ```

   For example, to configure for x86_64 with the default settings (using the Limine bootloader):

   ```bash
   ./env/configure x86_64
   ```

2. **Build the Kernel:**

   ***By default the build scripts runs with the max ammount of jobs (aka ur ammount of cores). To change this change $(nproc) in env/build to the ammount of jobs.***
   Execute the `./env/build` script to compile the kernel and generate the bootable ISO image.

   ```bash
   ./env/build <target> <bootloader>
   ```

   For building for the x86_64 target with the Limine bootloader:

   ```bash
   ./env/build x86_64 limine
   ```

   The resulting ISO image will be located in the `release/` directory.

3. **Run the Kernel (Optional):**

   Use the `./run.sh` script to automatically configure and run the kernel in QEMU.

   ```bash
   ./run.sh <target> <bootloader> [qemu args]
   ```

## Building Instructions (Docker)

1. **Build the Image:**

   Use the provided Dockerfile to set up an environment for building the kernel image.

   ```bash
   docker build -t leaf-builder \
       --build-arg TARGET=x86_64 \
       --build-arg BOOTLOADER=limine \
       .
   ```

2. **Create a Docker Container:**

   Create a Docker container from the built image.

   ```bash
   docker run --name leaf leaf-builder
   ```

3. **Copy the Built ISO to Your Local Machine:**

   Copy the ISO built within the Docker container to your local machine.

   ```bash
   docker cp leaf:/leaf/release/ .
   ```

   Now, the `release/` directory on your local machine (inside the Leaf directory) should contain the built ISO.

**You can also run the `docker_build.sh` script to automate this.**

## Key Features

- Interrupt Descriptor Table (IDT)
- Programmable Interrupt Controller (8259)
- Programmable Interval Timer (PIT)
- Physical Memory Manager (PMM)
- CPU Identification (CPUID)
- Serial Input/Output (I/O)
- Nighterm (Terminal emulator for TTY)
- TAR File Parsing
- Ramdisk (For copying files into memory using Limine modules)
- Virtual File System (VFS for reading and writing files)
- MADT (Multiple APIC Description Table)
- Symbol Table and lookup (stable)
- Backtrace

## Work in Progress

We are actively working on:

- Advanced Programmable Interrupt Controller (APIC) (IOAPIC and LAPIC)

## Supported Targets

- x86_64-limine
  - x86_64 with the Limine bootloader

## Requirements

Ensure you have the following tools installed:

- `nasm` (Assembler)
- `gcc` (Cross-compiler for the target architecture, e.g., x86_64-elf-gcc)
- `binutils` (Cross-assembler and linker, e.g., x86_64-elf-binutils)
- `cmake` (Build system)
- `ninja` (Build tool)
- `xorriso` (ISO creation tool)
- `qemu` (Emulator for testing the kernel)
- `docker` (For building using docker)

## Architectures

| Architecture | Status |
| ------------ | ------ |
| x86-64       | ✅     |
| x86          | ❌     |

## Bootloaders

| Bootloaders                                                  | Status |
| ------------------------------------------------------------ | ------ |
| [x86_64-limine](https://github.com/limine-bootloader/limine) | ✅     |
| [x86-leaf](https://github.com/leaf-kernel/bootloader)        | ❌     |

## Config

To change some config you can change the defines in `kernel/include/sys/_config.h`. Some defines are:
```c
#define __LEAF_VERBOSE__
```
*Enable verbose logging*

```c
#define __LEAF_VVERBOSE__
```
*Enable extra verbose logging*

## Credits

Credits to everyone who has helped and contributed to Leaf. With out the help of you all this wouldnt be possible.

## License

This project is licensed under the [MIT License](https://github.com/leaf-kernel/kernel/blob/main/LICENSE).

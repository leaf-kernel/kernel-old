# Leaf
A hobby kernel for nerds. Pre built ISO's [here](https://nightly.link/leaf-kernel/kernel/workflows/build/main/Leaf-release.zip)

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

   Execute the `./env/build` script to compile the kernel and generate the bootable ISO image.

   ```bash
   ./env/build <target> <bootloader>
   ```

   For building for the x86_64 target with the Limine bootloader:

   ```bash
   ./env/build x86_64
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

## License

This project is licensed under the [MIT License](https://github.com/leaf-kernel/kernel/blob/main/LICENSE).

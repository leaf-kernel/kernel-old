# Leaf
A hobby kernel.
## Building Instructions

To build a bootable image, follow these steps:

1. **Configure the Kernel:**

   Use the `./env/configure` script to set up the kernel build environment.

   ```bash
   ./env/configure <target> <bootloader> <toolchain>
   ```

   For example, to configure for x86_64 with the default settings (limine bootloader):

   ```bash
   ./env/configure x86_64
   ```

2. **Build the Kernel:**

   Execute the `./env/build` script to compile the kernel and generate the bootable ISO image.

   ```bash
   ./env/build <target> <bootloader>
   ```

   For building for the x86_64 target with limine bootloader:

   ```bash
   ./env/build x86_64
   ```

   The resulting ISO image will be located in the `release/` directory.

3. **Run the Kernel (Optional):**

   You can also use the `./run` script to automatically configure and run the kernel in QEMU.

   ```bash
   ./run <target> <bootloader> [qemu args]
   ```


## Key Features

- Interrupt Descriptor Table (IDT)
- Programmable Interrupt Controller (8259)
- Programmable Interval Timer (PIT)
- Physical Memory Manager (PMM)
- CPU Identification (CPUID)
- Serial Input/Output (I/O)
- Nighterm (Terminal emulator for TTY)
- TAR File Parsing
- Ramdisk (To copy files into memory, using limine modules)


## Work in Progress

We are actively working on:
- Advanced Programmable Interrupt Controller (APIC) (IOAPIC and LAPIC)


## Supported Targets
- x86_64-limine
    - x86_64 shipped with the limine bootloader.

## Requirements

Ensure you have the following tools installed:

- `nasm` (Assembler)
- `gcc` (Cross-compiler for the target architecture, e.g., x86_64-elf-gcc)
- `binutils` (Cross-assembler and linker, e.g., x86_64-elf-binutils)
- `cmake` (Build system)
- `xorriso` (ISO creation tool)
- `qemu` (Emulator for testing the kernel)


## Architectures

| Architecture | Status |
|--------------|--------|
| x86-64       | ✅     |
| x86          | ❌     |


## Bootloaders

| Bootloaders                                                   | Status |
|---------------------------------------------------------------|--------|
| [x86_64-limine](https://github.com/limine-bootloader/limine)  | ✅     |
| [x86-leaf](https://github.com/leaf-kernel/bootloader)         | ❌     |


## License

This project is licensed under the [MIT License](https://github.com/leaf-kernel/kernel/blob/main/LICENSE).

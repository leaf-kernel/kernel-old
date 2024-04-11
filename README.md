# Leaf Kernel
A x86_64 kernel.

## Building
To build an ISO. Just simply run:
```bash
make
```
You can also run and build using the provided `run.sh` script.
```
Usage: ./run.sh <target> [qemu args]
``` 

## Target List
- x86_64    (Supported)
- i386      (Unsupported)

## Requirements
- nasm
- Crosscompiler and binutils for target (x86_64-elf)
- xorriso

## Architectures
| Architecture 	| Status 	|
|--------------	|--------	|
| x86-64       	| ✅        |
| x86          	| ❌        |

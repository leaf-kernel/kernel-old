sudo apt update
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
export PATH="$PATH:/home/linuxbrew/.linuxbrew/bin/"; brew install wget unzip cmake ninja xorriso nasm x86_64-elf-gcc x86_64-elf-binutils
sudo apt install qemu-system

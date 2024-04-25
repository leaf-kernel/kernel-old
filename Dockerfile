FROM ubuntu:latest

ENV PATH="/home/linuxbrew/.linuxbrew/bin:${PATH}"

RUN apt-get update && \
    apt-get install -y build-essential curl file git unzip

RUN /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

RUN brew install cmake xorriso wget nasm ninja

ARG TARGET=x86_64
ARG BOOTLOADER=limine

RUN brew install ${TARGET}-elf-gcc ${TARGET}-elf-binutils

WORKDIR /leaf/

COPY . .

RUN ./env/configure ${TARGET} ${BOOTLOADER}
RUN ./env/build ${TARGET} ${BOOTLOADER}

CMD ["/bin/bash"]

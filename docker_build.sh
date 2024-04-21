#!/bin/bash
target=${1:-x86_64}
bootloader=${2:-limine}

echo ">>> Building docker image."
rm -rf release
docker rm leaf > /dev/null 2>&1

docker build -t leaf-builder \
    --build-arg TARGET=$target \
    --build-arg BOOTLOADER=$bootloader \
    .

echo ">> Creating container."
docker run --name leaf leaf-builder

echo ">> done."
docker cp leaf:/leaf/release/ .

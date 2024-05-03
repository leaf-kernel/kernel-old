#!/bin/bash
./run.sh x86_64 limine --display none --serial stdio -smp $(nproc) -m 2G ${@}
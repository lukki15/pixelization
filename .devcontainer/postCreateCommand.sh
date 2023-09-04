#!/bin/bash

sudo apt-get update
sudo apt-get install -y -q \
    gcc g++ \
    cmake make \
    doxygen \
    git curl zip \
    llvm pkg-config \
    unzip tar \
    python3 python3-pip \
    clang-format clang-tidy

pip install conan
conan profile detect

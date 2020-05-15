# Second State WebAssembly VM for EVMC Extension

The [Second State VM (SSVM)](https://github.com/second-state/ssvm) is a high performance WebAssembly runtime optimized for server side applications. This project provides support for Ewasm runtime which is compatible with [EVMC](https://github.com/ethereum/evmc). Please notice that SSVM-EVMC is not a standalone tool but a shared library which can initialize and execute by EVMC interface.

## NOTICE

The built library will be placed at `<your/build/folder>/tools/ssvm-evmc/libssvm-evmc.so` on Linux or `<your/build/folder>/tools/ssvm-evmc/libssvm-evmc.dylib` on MacOS.

# Getting Started

## Get Source Code

```bash
$ git clone git@gitlab.com:secondstate/vm/ssvm-evmc.git
$ cd ssvm-evmc
$ git checkout master
```

## Prepare environment

### Use our docker image

Our docker image use `ubuntu 18.04` as base.

```bash
$ docker pull secondstate/ssvm
```

### Or setup the environment manually

```bash
$ sudo apt install -y \
	cmake \
	gcc-8 \
	g++-8
	libboost-all-dev
# And you will need to install llvm-9 for ssvm-aot tools
$ wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
$ sudo apt update && apt install -y \
	libllvm9 \
	llvm-9 \
	llvm-9-dev \
	llvm-9-runtime \
	libclang-common-9-dev # for yaml-bench

```

## Build SSVM-EVMC

### Enter the build path

```bash
$ cd <path/to/ssvm-evmc>
$ mkdir -p build && cd build
```

### Prepare ssvm-core and build with it

Prepare environment from [SSVM](https://github.com/second-state/SSVM) page.

```bash
$ cmake -DSSVM_CORE_PATH=<path/to/ssvm/source> -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .. && make
```

### Automatically get ssvm-core

```bash
$ cmake  -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .. && make
```

## Run built-in tests

The following built-in tests are only avaliable when the build flag `BUILD_TESTS` sets to `ON`.

```bash
$ cd <path/to/ssvm/build_folder>
$ cd test/evmc
$ ./ssvmEVMCTest
```

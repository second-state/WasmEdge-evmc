# Second State WebAssembly VM for EVMC Extension

The [Second State VM (SSVM)](https://github.com/second-state/ssvm) is a high-performance WebAssembly runtime optimized for server-side applications. This project provides support for Ewasm runtime which is compatible with [EVMC](https://github.com/ethereum/evmc). Please notice that SSVM-EVMC is not a standalone tool but a shared library that can initialize and execute by the EVMC interface.

## NOTICE

The built library will be placed at `<your/build/folder>/tools/ssvm-evmc/libssvm-evmc.so` on Linux or `<your/build/folder>/tools/ssvm-evmc/libssvm-evmc.dylib` on MacOS.

# Getting Started

## Get SSVM-EVMC Source Code

```bash
$ git clone git@github.com:second-state/ssvm-evmc.git
$ cd ssvm-evmc
$ git checkout master
```

## Prepare the environment


### Use our docker image (Recommand)

Our docker image is based on `ubuntu 18.04`.

```bash
$ docker pull secondstate/ssvm
```

### Or setup the environment manually

Please notice that SSVM-EVMC requires cmake>=3.11

```bash
$ sudo apt install -y \
	git \
	cmake \
	gcc-8 \
	g++-8 \
	libboost-all-dev
# And you will need to install llvm-9 for ssvm-aot tools
$ wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
$ sudo apt update && apt install -y \
        libllvm9 \
        llvm-9 \
        llvm-9-dev \
        liblld-9-dev \
        llvm-9-runtime \
        libclang-common-9-dev # for yaml-bench
```

## Build SSVM-EVMC

### Create and enter the build folder

```bash
$ cd <path/to/ssvm-evmc>
$ mkdir -p build && cd build
```

### Build SSVM and SSVM-EVMC

SSVM-EVMC depends on SSVM. Please refer to [SSVM Project Repo](https://github.com/second-state/SSVM) to get the source code and build it.

We provide two approaches to build SSVM-Core:

#### Option  1. Build SSVM from source code

```bash
$ git clone git@github.com:second-state/SSVM.git <path/to/ssvm/source/folder>
$ cmake -DSSVM_CORE_PATH=<path/to/ssvm/source/folder> -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .. && make
```

#### Option 2. Use our built-in cmake rule to fetch SSVM

```bash
$ cmake  -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON .. && make
```

## Run built-in tests

The following built-in tests are only available when the build flag `BUILD_TESTS` sets to `ON`.

```bash
$ cd <path/to/ssvm/build/folder>
$ cd test/evmc
$ ./ssvmEVMCTest
```

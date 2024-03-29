# WasmEdge for EVMC Extension

The [WasmEdge (previously known as Second State VM, SSVM)](https://github.com/WasmEdge/WasmEdge) is a high-performance WebAssembly runtime optimized for server-side applications. This project provides support for Ewasm runtime which is compatible with [EVMC](https://github.com/ethereum/evmc). Please notice that WasmEdge-EVMC is not a standalone tool but a shared library that can initialize and execute by the EVMC interface.

## NOTICE

The built library will be placed at `<your/build/folder>/tools/wasmedge-evmc/libwasmedge-evmc.so` on Linux or `<your/build/folder>/tools/wasmedge-evmc/libwasmedge-evmc.dylib` on MacOS.

# Getting Started

## Get WasmEdge-EVMC Source Code

```bash
$ git clone git@github.com:second-state/wasmedge-evmc.git
$ cd wasmedge-evmc
```

## Prepare the environment


### Use our docker image (Recommand)

Our docker image is based on `ubuntu 20.04`.

```bash
$ docker pull wasmedge/wasmedge
```

### Or setup the environment manually

Please notice that WasmEdge-EVMC requires cmake>=3.11

```bash
# Tools and libraries
$ sudo apt install -y \
	software-properties-common \
	cmake \
	libboost-all-dev

# And you will need to install llvm for wasmedge-aot tools
$ sudo apt install -y \
	llvm-dev \
	liblld-10-dev

# WasmEdge supports both clang++ and g++ compilers
# You can choose one of them for building this project
$ sudo apt install -y gcc g++
$ sudo apt install -y clang
```

## Build WasmEdge-EVMC

### Create and enter the build folder

```bash
$ cd <path/to/wasmedge-evmc>
$ mkdir -p build && cd build
```

### Build WasmEdge and WasmEdge-EVMC

WasmEdge-EVMC depends on WasmEdge. Please refer to [WasmEdge Project](https://github.com/WasmEdge/WasmEdge) to get the source code and build it.

#### Use the built-in cmake rule to fetch WasmEdge

```bash
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make -j
```

## Run built-in tests

```bash
$ cd <path/to/wasmedge/build/folder>
$ cd test/evmc
$ ./wasmedgeEVMCTest
```

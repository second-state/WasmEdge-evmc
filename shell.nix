let
  pkgs = import <nixpkgs> { };
  clangStdenv = pkgs.llvmPackages_10.stdenv;
in
clangStdenv.mkDerivation {
  name = "clang-10-nix-shell";
  buildInputs = with pkgs; [
    llvmPackages_10.llvm
    cmake
    pkg-config
    lld_10
    boost

    # Current build error message from cmake is
    # ```
    # /nix/store/5xyjd2qiily84lcv2w2grmwsb8r1hqpr-binutils-2.35.1/bin/ld: cannot find -ldl
    # /nix/store/5xyjd2qiily84lcv2w2grmwsb8r1hqpr-binutils-2.35.1/bin/ld: cannot find -lrt
    # /nix/store/5xyjd2qiily84lcv2w2grmwsb8r1hqpr-binutils-2.35.1/bin/ld: cannot find -lm
    # /nix/store/5xyjd2qiily84lcv2w2grmwsb8r1hqpr-binutils-2.35.1/bin/ld: cannot find -lpthread
    # /nix/store/5xyjd2qiily84lcv2w2grmwsb8r1hqpr-binutils-2.35.1/bin/ld: cannot find -lc
    # ```
    #
    # TODO: fix this
    # glibc.static
    # gcc
  ];

  # TODO:
  # cmake using llvm-config or something like this
  # figure out how to do this and remove the llvmPackages_10.llvm in buildInputs
  # LLVM_DIR="${pkgs.llvmPackages_10.llvm.out}/lib/cmake/llvm";
}

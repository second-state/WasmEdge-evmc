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
  ];
}

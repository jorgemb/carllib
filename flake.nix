{
  description = "C++ development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-26.05";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          gcc
          clang-tools # clangd, clang-format, etc.
          cmake
          cppcheck
          gnumake
          ninja # optional but recommended
          pkg-config

	  # Base dependencies
          catch2_3
          fmt
          spdlog


          # Project dependencies
	  tomlplusplus
          sfml
        ];

        # Tell CMake to use gcc
        shellHook = ''
          export CC=gcc
          export CXX=g++
        '';
      };
    };
}

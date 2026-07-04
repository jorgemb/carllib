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
          gcc16
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
	  libGL
	  libGLU
	  glm 		# OpenGL math library

	  cli11		# Command line parsing
        ];

        # Tell CMake to use gcc
        shellHook = ''
          export CC=gcc
          export CXX=g++
        '';
      };
    };
}

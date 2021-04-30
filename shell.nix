with import <nixpkgs> { };
stdenv.mkDerivation {
  name = "simulant-dev-environment";
  buildInputs = [ pkg-config cmake doctest SDL2 SDL2_image SDL2_ttf];
}

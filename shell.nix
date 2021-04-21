with import <nixpkgs> {};
stdenv.mkDerivation {
    name = "simulant-dev-environment";
    buildInputs = [ pkg-config cmake doctest ];
}

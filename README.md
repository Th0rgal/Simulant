# Simulant

A simulation of ant colonies realized in C++.

## Features
- Support for multiple colonies
- Real time rendering with SDL2
- Optimization of the rendering with the graphic card
- Dynamic updating of entities and pheromones

## Code conventions

#### Docstrings
We followed javadocs guidelines with an additional decorator:
``@confidence 0-5``
- 0: it doesn't work
- 1: it "works" with known bugs
- 3: it works but can't be tested
- 4: it works and has been been tested
- 5: it works, it has been tested and it is so simple that there is no doubt about it

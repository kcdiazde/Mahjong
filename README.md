# Mahjong
Mahjong 2D simulator based on the Classical chinese game.

Inspired in classical mahjong but played with local rules this game was developed as a side project.

<img width="1940" height="1135" alt="MahjongScreen" src="https://github.com/user-attachments/assets/a3f600df-ced2-454a-87af-74e49323797b" />

## Implementation details
- Modern c++
- Built with Cmake
- With unit-testing infra based on ctest
- Uses minimum external libraries besides SFML (game engine).

![image](https://github.com/user-attachments/assets/dfbfdd58-40a1-4527-b893-ce110c70093d)

## To build
cmake -B build
cmake --build build

## To recompile
cd build
cmake --build .

## To run
cd build
./bin/main

## To ctest
ctest -V
ctest -R <test name> -V

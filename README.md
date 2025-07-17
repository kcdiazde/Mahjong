# Mahjong
Mahjong 2D simulator based on the Classical chinese game

![image](https://github.com/user-attachments/assets/dfbfdd58-40a1-4527-b893-ce110c70093d)

## To build
cmake -B build
cmake --build build

## To recompile
cd build
cmake --build .

# To run
cd build
./bin/main

# To ctest
ctest -V
ctest unit_tests -V



To build for host:
yt target x86-osx-native

To build for micro:bit
yt target bbc-microbit-classic-gcc


Remote debugging instructions:
https://os.mbed.com/users/maclobdell/notebook/debugging-with-pyocd/

To build test that run on the host:
1) cd native-tests
2) cmake -DCMAKE_BUILD_TYPE=Debug .
3) make

yotta will not generate a CMakeList.txt file if one exists already. So,
we use a custom CMakeList.txt file to build tests that don't need to be
compiled for the micro:bit.

Note that in the modules.json file, we only depend on the microbit runtime
when compiling for a microbit target. This avoids build errors when compiling
for a native target, for which we provide stub implementations of the microbit
runtime classes.

To run unit tests:
yt target x86-osx-native
yt test 

# Installation
## List of dependencies
* [CMake 3.12+](https://cmake.org/) 
* [Google Protobuf, version 3.0.0+](https://github.com/protocolbuffers/protobuf) 
* [Qt5](https://www.qt.io/)
* [Bullet3](https://github.com/bulletphysics/bullet3)

To run the unit tests you also need the following:
* [Google Test](https://github.com/google/googletest)
## Installation on Linux (tested on Ubuntu 18.04)
### CMake

```
sudo apt-get install cmake
```
### Google Protobuf

### Qt5

### Bullet3
Not yet relevant. Just install the library from https://github.com/bulletphysics/bullet3.
~~On linux: It's important to have the correct drivers installed. For Nvidia users; run
```sudo apt-get install nvidia-driver-430``` (as of writing the latest version) and reboot.
Your application will compile and run without these but if you recompile after installation the physics simulator should utilise your GPU much better.~~

### Google Test
To obtain the library run:
 
 ``` sudo apt-get install libgtest-dev```

In order to actually build the library you have to build it yourself.
```
sudo apt-get install cmake # install cmake
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make

# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib

```

# Name
In Norse mythology the tree Yggdrasil makes up the entire world. At the root of the tree Yggdrasil there is a well. The water from this well contains all the knowledge and wisdom in the world, and feeds Yggdrasil. A mythical creature called Mímir guards the well, and drinks from it every day. 
The god Odin famously sacrificed an eye in order to be able to drink from Mímir's well.
As Mímir is not a god, he is mortal; thus he is beheaded in a war. After his death, Odin carries around Mímir's head and asks it for secret knowledge and advice.

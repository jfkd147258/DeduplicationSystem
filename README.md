@ 2023/06/13 by Chun-Le Yeh

#Introduction
This is a simple implementation of a deduplication system, including the functionalities of chunking and fingerprinting. It is developed using C++ as the programming language. The chunking method used is content-defined chunking, and the fingerprinting is performed using SHA-256 hashing.

#Development Environment Requirements
Development Environment Requirements

The current setup for this project is running on a host machine with Ubuntu 20.04. Please ensure that the following requirements are met:
* 1. Operating System: Ubuntu 20.04
* 2. GCC: Make sure that GCC (GNU Compiler Collection) is installed on the system.
* 3. Future Development: The upcoming features will be specifically targeted for UPMEM DPU.

#Test
Ensure that the data you want to deduplicate is correctly placed in the directory and named as "Data1".

run
```
make
```

```
make run
```

after testing
```
make clean
```

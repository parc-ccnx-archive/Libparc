Libparc
=======
The PARC C Library

## Quick Start ##
```
$ git clone git@github.com:PARC/Libparc.git Libparc
$ mkdir Libparc.build
$ cd Libparc.build
$ cmake -DCMAKE_INSTALL_PREFIX=${INSTALATION_DIRECTORY} ../Libparc
$ make
$ make test
$ make install
```

## Introduction ##

The PARC Library is a C runtime providing an array of features and capabilities for C programs and programmers.

Functionality is grouped into:

* Data structures
* Input Output
* Memory and Buffer Management
* Threading and Concurrency
* Security
* Developer Aids
* Networking and Communication

## Using Libparc ##

### Distillery ###

Libparc is part of [CCNx Distillery](https://github.com/PARC/CCNx_Distillery). You may want to get the software via that distribution if you want to work on CCNx.

### Platforms ###

Libparc has been tested in:

- Ubuntu 14.04 (x86_64)
- MacOSX 10.10
- MacOSX 10.11

Other platforms and architectures may work.

### Dependencies ###

Build dependencies:

- c99 ( clang / gcc )
- CMake 3.4

Basic dependencies:

- OpenSSL
- pthreads
- Libevent
- [LongBow](https://github.com/PARC/LongBow)

Documentation dependencies:

- Doxygen


### Getting Started ###

Libparc is built using cmake. You will need to have CMake 3.4 installed in order to build it.

```
Download Libparc
$ git clone git@github.com:PARC/Libparc.git Libparc

Create build directory
$ mkdir Libparc.build
$ cd Libparc.build

Prepare the build, give an install directory
$ cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ../Libparc

Compile the software
$ make

Run unit tests
$ make test

Install the software
$ make install
```

This will create the `include` and `lib` directories containing the necessary files to compile with LongBow in the `${INSTALL_DIR}`.



### Using Libparc ###

Libparc is a set of functions and data structures for C. You can use it in your code by including the right header files and linking to the libparc library.

```
LIBPARC=<directory-where-libparc-is-installed>

 -I${LIBPARC}/include -L${LIBPARC}/lib -lparc
```

### Contact ###

- [Libparc GitHub](https://github.com/PARC/Libparc)
- [CCNx Website](http://www.ccnx.org/)
- [CCNx Mailing List](https://www.ccnx.org/mailman/listinfo/ccnx/)


### License ###

This software is distributed under the following license:

```
Copyright (c) 2013, 2014, 2015, 2016, Xerox Corporation (Xerox)and Palo Alto
Research Center (PARC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Patent rights are not granted under this agreement. Patent rights are
  available under FRAND terms.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

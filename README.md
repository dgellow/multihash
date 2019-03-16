# Multihash in C++ [![Build Status](https://travis-ci.org/dgellow/multihash.svg?branch=master)](https://travis-ci.org/dgellow/multihash)

C++ library to encode/decode [multihash digests](https://multiformats.io/multihash/).

## Install

```
$ git clone https://github.com/dgellow/multihash.git
$ cd multihash
$ mkdir build
$ cd build
$ cmake ..
$ make && make test ARGS="-V" && make install
```

## Usage

See [unit tests](https://github.com/dgellow/multihash/blob/master/tests/multihash_test.cpp) for usage examples.
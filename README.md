
# gtl #

This repository is a collection of useful cross-platform, well-commented, self-contained, simple, and interesting C++ classes in the style of the standard template library.

[![Build Status](https://github.com/gpdaniels/gtl/actions/workflows/main.yml/badge.svg)](https://github.com/gpdaniels/gtl/actions/workflows/main.yml) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/gpdaniels/gtl.svg)](https://lgtm.com/projects/g/gpdaniels/gtl/context:cpp) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) 

## Content ##

The current classes are as described below:

| Group      |                Class | Description                                                    | :grey_question: |
|:-----------|---------------------:|:---------------------------------------------------------------|:---------------:|
| [ai](source/ai) | [ann](source/ai/ann) | Artificial neural network using the backpropagation algorithm for training. | :heavy_check_mark: |
| [ai](source/ai) | [hmm](source/ai/hmm) | Hidden markov model using the Baum-Welch algorithm for training. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [gaussian_elimination](source/algorithm/gaussian_elimination) | Linear simultaneous equation solver. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [quicksort](source/algorithm/quicksort) | Recursive quicksort algorithm, both full and partial versions. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [simulation_loop](source/algorithm/simulation_loop) | Fixed time step helper class for creating game loops. | :heavy_check_mark: |
| [container](source/container) | [any](source/container/any) | Class that can hold any variable type. | :heavy_check_mark: |
| [container](source/container) | [array_nd](source/container/array_nd) | N-dimensional statically or dynamically sized array. | :heavy_check_mark: |
| [container](source/container) | [lambda](source/container/lambda) | Lambda function class that uses the heap for storage. | :heavy_check_mark: |
| [container](source/container) | [ring_buffer](source/container/ring_buffer) | Statically sized thread-safe multi-producer multi-consumer ring-buffer. | :heavy_check_mark: |
| [container](source/container) | [static_array_nd](source/container/static_array_nd) | N-dimensional statically sized array. | :heavy_check_mark: |
| [container](source/container) | [static_lambda](source/container/static_lambda) | Lambda function class that uses the stack for storage. | :heavy_check_mark: |
| [crypto](source/crypto) | [aes](source/crypto/aes) | An implementation of the aes encryption algorithm for 128, 196, and 256 bits. | :heavy_check_mark: |
| [debug](source/debug) | [access](source/debug/access) | Classes and macros for accessing class private members. | :heavy_check_mark: |
| [debug](source/debug) | [assert](source/debug/assert) | Macros that define an assert macro that optionally takes a format string and parameters. | :heavy_check_mark: |
| [debug](source/debug) | [assume](source/debug/assume) | Macro that hints to the compiler when a statement should be assumed true. | :heavy_check_mark: |
| [debug](source/debug) | [breakpoint](source/debug/breakpoint) | Macro for creating a breakpoint. | :heavy_check_mark: |
| [debug](source/debug) | [short_file](source/debug/short_file) | Replacement for the __FILE__ macro to prevent build path being leaked into the binary. | :heavy_check_mark: |
| [debug](source/debug) | [signal](source/debug/signal) | Class to wrap signal handlers allowing the use of lambdas with scope. | :heavy_check_mark: |
| [debug](source/debug) | [unused](source/debug/unused) | Macro for hiding unused variable warnings. | :heavy_check_mark: |
| [execution](source/execution) | [barrier](source/execution/barrier) | Thread syncronisation barrier. | :heavy_check_mark: |
| [execution](source/execution) | [coroutine](source/execution/coroutine) | Setjump/Longjump implementation of stackful coroutines. | :heavy_check_mark: |
| [execution](source/execution) | [semaphore](source/execution/semaphore) | Semaphore made using a mutex and condition variable. | :heavy_check_mark: |
| [execution](source/execution) | [spin_lock](source/execution/spin_lock) | Spin lock implemented using an atomic flag. | :heavy_check_mark: |
| [execution](source/execution) | [thread_pool](source/execution/thread_pool) | Multi-queue thread-pool that performs jobs in priority order. | :heavy_check_mark: |
| [execution](source/execution) | [triple_buffer](source/execution/triple_buffer) | Lockless triple buffer interface to three buffers. | :heavy_check_mark: |
| [hash](source/hash) | [crc](source/hash/crc) | An implementation of the crc hashing function for 8, 16, 32, and 64 bits. | :heavy_check_mark: |
| [hash](source/hash) | [md5](source/hash/md5) | An implementation of the md5 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha0](source/hash/sha0) | An implementation of the sha0 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha1](source/hash/sha1) | An implementation of the sha1 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha2](source/hash/sha2) | An implementation of the sha2 hashing function for 224, 256, 384, and 512 bits. | :heavy_check_mark: |
| [hash](source/hash) | [sha3](source/hash/sha3) | An implementation of the sha3 hashing function for 224, 256, 384, and 512 bits. | :heavy_check_mark: |
| [io](source/io) | [paths](source/io/paths) | Collection of cross platform functions to provide useful paths. | :heavy_check_mark: |
| [math](source/math) | [big_integer](source/math/big_integer) | Arbitrary sized signed integers. | :heavy_check_mark: |
| [math](source/math) | [big_unsigned](source/math/big_unsigned) | Arbitrary sized unsigned integers. | :heavy_check_mark: |
| [platform](source/platform) | [cpu](source/platform/cpu) | Class to extract cpuid information to determine supported instructions at runtime. | :heavy_check_mark: |
| [protection](source/protection) | [virtual_machine](source/protection/virtual_machine) | A simple stack based virtual machine allowing easy creation of custom operands. | :heavy_check_mark: |
| [random](source/random) | [random_mt](source/random/random_mt) | Mersenne twister pseudo-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_pcg](source/random/random_pcg) | PCG pseudo-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_romu_32](source/random/random_romu_32) | Romu 32 bit pseudo-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_romu_64](source/random/random_romu_64) | Romu 64 bit pseudo-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_splitmix](source/random/random_splitmix) | Splitmix pseudo-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_xorshift](source/random/random_xorshift) | Xorshift pseudo-random number generator. | :heavy_check_mark: |
| [string](source/string) | [static_string_encrypted](source/string/static_string_encrypted) | Compile-time string encryption with runtime decryption to obfuscate strings. | :heavy_check_mark: |
| [type](source/type) | [enum_name](source/type/enum_name) | Compile-time enum value name as a string with -fno-rtti. | :heavy_check_mark: |
| [type](source/type) | [type_id](source/type/type_id) | Simple run-time-type-information (rtti) with -fno-rtti. | :heavy_check_mark: |
| [type](source/type) | [type_name](source/type/type_name) | Compile-time type name as a string with -fno-rtti. | :heavy_check_mark: |
| [utility](source/utility) | [error](source/utility/error) | Composable error class that can be returned down a call stack. | :heavy_check_mark: |
| [utility](source/utility) | [event](source/utility/event) | Thread safe multi-in/multi-out event/messaging system. | :heavy_check_mark: |
| [utility](source/utility) | [pimpl](source/utility/pimpl) | Statically sized pointer to implementation wrapper. | :heavy_check_mark: |

## License ##

The MIT License

Copyright (c) 2019 Geoffrey Daniels. http://gpdaniels.com/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

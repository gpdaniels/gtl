
# gtl #

This repository is a collection of useful cross-platform, well-commented, self-contained, simple, and interesting C++ classes in the style of the standard template library.

[![Build Status](https://github.com/gpdaniels/gtl/actions/workflows/main.yml/badge.svg)](https://github.com/gpdaniels/gtl/actions/workflows/main.yml) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/gpdaniels/gtl.svg)](https://lgtm.com/projects/g/gpdaniels/gtl/context:cpp) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) 

## Content ##

The current classes are as described below:

### AI ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                     **ann** | Artificial neural network using the backpropagation algorithm for training.             |
|                     **hmm** | Hidden markov model using the Baum-Welch algorithm for training.                        |

### Algorithm ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|    **gaussian_elimination** | Linear simultaneous equation solver.                                                    |
|         **simulation_loop** | Fixed time step helper class for creating game loops.                                   |
|               **quicksort** | Recursive quicksort algorithm, both full and partial versions.                          |

### Container ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                     **any** | Class that can hold any variable type.                                                  |
|                **array_nd** | N-dimensional statically or dynamically sized array.                                    |
|                  **lambda** | Lambda function class that uses the heap for storage.                                   |
|             **ring_buffer** | Statically sized thread-safe multi-producer multi-consumer ring-buffer.                 |
|         **static_array_nd** | N-dimensional statically sized array.                                                   |
|           **static_lambda** | Lambda function class that uses the stack for storage.                                  |

### Crypto ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                     **aes** | An implementation of the aes encryption algorithm for 128, 196, and 256 bits.           |

### Debug ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                  **access** | Classes and macros for accessing class private members.                                 |
|                  **assert** | Macros that define an assert macro that optionally takes a format string and parameters.|
|                  **assume** | Macro that hints to the compiler when a statement should be assumed true.               |
|              **breakpoint** | Macro for creating a breakpoint.                                                        |
|                  **signal** | Class to wrap signal handlers allowing the use of lambdas with scope.                   |
|              **short_file** | Replacement for the __FILE__ macro to prevent build path being leaked into the binary.  |
|                  **unused** | Macro for hiding unused variable warnings.                                              |

### Execution ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                 **barrier** | Thread syncronisation barrier.                                                          |
|               **coroutine** | Setjump/Longjump implementation of stackful coroutines.                                 |
|               **semaphore** | Semaphore made using a mutex and condition variable.                                    |
|               **spin_lock** | Spin lock implemented using an atomic flag.                                             |
|           **triple_buffer** | Lockless triple buffer interface to three buffers.                                      |
|             **thread_pool** | Multi-queue thread-pool that performs jobs in priority order.                           |

### Hash ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                     **crc** | An implementation of the crc hashing function for 8, 16, 32, and 64 bits.               |
|                     **md5** | An implementation of the md5 hashing function.                                          |
|                    **sha0** | An implementation of the sha0 hashing function.                                         |
|                    **sha1** | An implementation of the sha1 hashing function.                                         |
|                    **sha2** | An implementation of the sha2 hashing function for 224, 256, 384, and 512 bits.         |
|                    **sha3** | An implementation of the sha3 hashing function for 224, 256, 384, and 512 bits.         |

### IO ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                   **paths** | Collection of cross platform functions to provide useful paths.                         |

### Math ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|             **big_integer** | Arbitrary sized signed integers.                                                        |
|            **big_unsigned** | Arbitrary sized unsigned integers.                                                      |

### Protection ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|         **virtual_machine** | A simple stack based virtual machine allowing easy creation of custom operands.         |

### Random ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|               **random_mt** | Mersenne twister pseudo-random number generator.                                        |
|              **random_pcg** | PCG pseudo-random number generator.                                                     |
|          **random_romu_32** | Romu 32 bit pseudo-random number generator.                                             |
|          **random_romu_64** | Romu 64 bit pseudo-random number generator.                                             |
|         **random_splitmix** | Splitmix pseudo-random number generator.                                                |
|         **random_xorshift** | Xorshift pseudo-random number generator.                                                |

### Strings ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
| **static_string_encrypted** | Compile-time string encryption with runtime decryption to obfuscate strings.            |

### Type Information ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|               **enum_name** | Compile-time enum value name as a string with -fno-rtti.                                |
|                 **type_id** | Simple run-time-type-information (rtti) with -fno-rtti.                                 |
|               **type_name** | Compile-time type name as a string with -fno-rtti.                                      |

### Utility ###

|                       Class | Description                                                                             |
|----------------------------:|:----------------------------------------------------------------------------------------|
|                     **cpu** | Class to extract cpuid information to determine supported instructions at runtime.      |
|                   **error** | Composable error class that can be returned down a call stack.                          |
|                   **event** | Thread safe multi-in/multi-out event/messaging system.                                  |
|                   **pimpl** | Statically sized pointer to implementation wrapper.                                     |

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

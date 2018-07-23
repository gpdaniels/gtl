
# gtl #

This repository aims is a collection of useful well-commented, self-contained, simple, and/or interesting C++ classes in the style of the standard template library.

The current classes are as described below:

| Class               | Description                                              |
|--------------------:|:---------------------------------------------------------|
| **access**          | Classes and macros for accessing class private members.  |
| **any**             | A class that can hold any variable type.                 |
| **array_nd**        | N-dimensional staticly sized array.                      |
| **barrier**         | Thread syncronisation barrier.                           |
| **coroutine**       | Setjump/longjump implementation of stackful coroutines.  |
| **enum_name**       | Compile-time enum value name as a string with -fno-rtti. |
| **event**           | A thread safe multi-in/multi-out event/messaging system. |
| **pimpl**           | A statically sized pointer to implementation wrapper.    |
| **random_mt**       | Mersenne twister pseudo-random number generator.         | 
| **semaphore**       | Semaphore made using a mutex and condition variable.     | 
| **simulation_loop** | Fixed time step helper class for creating game loops.    | 
| **spin_lock**       | Spin lock implemented using an atomic flag.              | 
| **type_id**         | Simple run-time-type-information (rtti) with -fno-rtti.  |
| **type_name**       | Compile-time type name as a string with -fno-rtti.       |

## License ##

The MIT License

Copyright (c) 2018 Geoffrey Daniels. http://gpdaniels.com/

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

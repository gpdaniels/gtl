
# gtl #

This repository is a collection of useful cross-platform, well-commented, self-contained, simple, and interesting C++ classes in the style of the standard template library.

[![Build Status](https://github.com/gpdaniels/gtl/actions/workflows/main.yml/badge.svg)](https://github.com/gpdaniels/gtl/actions/workflows/main.yml) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/gpdaniels/gtl.svg)](https://lgtm.com/projects/g/gpdaniels/gtl/context:cpp) [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT) 

## Content ##

The current classes are as described below:

| Group      |                Class | Description                                                    | :grey_question: |
|:-----------|---------------------:|:---------------------------------------------------------------|:---------------:|
| [ai](source/ai) | [ann](source/ai/ann) | Artificial neural network using the backpropagation algorithm for training. | :heavy_check_mark: |
| [ai](source/ai) | [hmm](source/ai/hmm) | Hidden markov model using the Baum\-Welch algorithm for training. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [dijkstra](source/algorithm/dijkstra) | Implementation of Dijkstra's algorithm used to solve pathfinding problems. | :construction: |
| [algorithm](source/algorithm) | [gaussian_elimination](source/algorithm/gaussian_elimination) | Linear simultaneous equation solver. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [kmeans](source/algorithm/kmeans) | Implementation of the KMeans clustering algorithm. | :construction: |
| [algorithm](source/algorithm) | [maxn](source/algorithm/maxn) | The MaxN search algorithm to find the best possible moves for an N player game with known information. | :construction: |
| [algorithm](source/algorithm) | [quicksort](source/algorithm/quicksort) | Recursive quicksort algorithm, both full and partial versions. | :heavy_check_mark: |
| [algorithm](source/algorithm) | [satisfiability](source/algorithm/satisfiability) | A simple SAT solver. | :construction: |
| [algorithm](source/algorithm) | [simulation_loop](source/algorithm/simulation_loop) | Fixed time step helper class for creating game loops. | :heavy_check_mark: |
| [container](source/container) | [any](source/container/any) | Class that can hold any variable type. | :heavy_check_mark: |
| [container](source/container) | [array_nd](source/container/array_nd) | N\-dimensional statically or dynamically sized array. | :heavy_check_mark: |
| [container](source/container) | [lambda](source/container/lambda) | Lambda function class that uses the heap for storage. | :heavy_check_mark: |
| [container](source/container) | [ring_buffer](source/container/ring_buffer) | Dynamically sized thread\-safe multi\-producer multi\-consumer ring\-buffer. | :heavy_check_mark: |
| [container](source/container) | [static_array_nd](source/container/static_array_nd) | N\-dimensional statically sized array. | :heavy_check_mark: |
| [container](source/container) | [static_lambda](source/container/static_lambda) | Lambda function class that uses the stack for storage. | :heavy_check_mark: |
| [container](source/container) | [static_ring_buffer](source/container/static_ring_buffer) | Statically sized thread\-safe multi\-producer multi\-consumer ring\-buffer. | :heavy_check_mark: |
| [container](source/container) | [static_variant](source/container/static_variant) | A static\_variant class that can contain any one of its listed template types. | :construction: |
| [container](source/container) | [static_view](source/container/static_view) | A non\-owning static\_view into multi\-dimensional memory. | :construction: |
| [crypto](source/crypto) | [aes](source/crypto/aes) | An implementation of the aes encryption algorithm for 128, 196, and 256 bits. | :heavy_check_mark: |
| [crypto](source/crypto) | [rc4](source/crypto/rc4) |  | :construction: |
| [debug](source/debug) | [access](source/debug/access) | Classes and macros for accessing class private members. | :heavy_check_mark: |
| [debug](source/debug) | [assert](source/debug/assert) | Macros that define an assert macro that optionally takes a format string and parameters. | :heavy_check_mark: |
| [debug](source/debug) | [assume](source/debug/assume) | Macro that hints to the compiler when a statement should be assumed true. | :heavy_check_mark: |
| [debug](source/debug) | [breakpoint](source/debug/breakpoint) | Macro for creating a breakpoint. | :heavy_check_mark: |
| [debug](source/debug) | [expect](source/debug/expect) | Macro that hints to the compiler when a statement should be expected as true or false. | :construction: |
| [debug](source/debug) | [sentinel](source/debug/sentinel) | A simple class to help detect stack overflows. | :construction: |
| [debug](source/debug) | [short_file](source/debug/short_file) | Replacement for the \_\_FILE\_\_ macro to prevent build path being leaked into the binary. | :heavy_check_mark: |
| [debug](source/debug) | [signal](source/debug/signal) | Class to wrap signal handlers allowing the use of lambdas with scope. | :heavy_check_mark: |
| [debug](source/debug) | [unused](source/debug/unused) | Macro for hiding unused variable warnings. | :heavy_check_mark: |
| [execution](source/execution) | [barrier](source/execution/barrier) | Thread syncronisation barrier. | :heavy_check_mark: |
| [execution](source/execution) | [coroutine](source/execution/coroutine) | Setjump/Longjump implementation of stackful coroutines. | :heavy_check_mark: |
| [execution](source/execution) | [semaphore](source/execution/semaphore) | Semaphore made using a mutex and condition variable. | :heavy_check_mark: |
| [execution](source/execution) | [spin_lock](source/execution/spin_lock) | Spin lock implemented using an atomic flag. | :heavy_check_mark: |
| [execution](source/execution) | [thread_pool](source/execution/thread_pool) | Multi\-queue thread\-pool that performs jobs in priority order. | :heavy_check_mark: |
| [execution](source/execution) | [triple_buffer](source/execution/triple_buffer) | Lockless triple buffer interface to three buffers. | :heavy_check_mark: |
| [file/archive](source/file/archive) | [tar](source/file/archive/tar) | Tar format archive reader and writer. | :construction: |
| [file/text](source/file/text) | [json](source/file/text/json) | A small json parser and composer. | :construction: |
| [game](source/game) | [sudoku](source/game/sudoku) | A sudoku solver for standard 9x9 grids. | :construction: |
| [game](source/game) | [tic_tac_toe](source/game/tic_tac_toe) | Solver for the game tic\-tac\-toe on a 3x3 board. | :construction: |
| [hash](source/hash) | [crc](source/hash/crc) | An implementation of the crc hashing function for 8, 16, 32, and 64 bits. | :heavy_check_mark: |
| [hash](source/hash) | [md5](source/hash/md5) | An implementation of the md5 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha0](source/hash/sha0) | An implementation of the sha0 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha1](source/hash/sha1) | An implementation of the sha1 hashing function. | :heavy_check_mark: |
| [hash](source/hash) | [sha2](source/hash/sha2) | An implementation of the sha2 hashing function for 224, 256, 384, and 512 bits. | :heavy_check_mark: |
| [hash](source/hash) | [sha3](source/hash/sha3) | An implementation of the sha3 hashing function for 224, 256, 384, and 512 bits. | :heavy_check_mark: |
| [io](source/io) | [file](source/io/file) | An RAII file handle that wraps file operation functions. | :construction: |
| [io](source/io) | [paths](source/io/paths) | Collection of cross platform functions to provide useful paths. | :heavy_check_mark: |
| [math](source/math) | [big_integer](source/math/big_integer) | Arbitrary sized signed integers. | :heavy_check_mark: |
| [math](source/math) | [big_unsigned](source/math/big_unsigned) | Arbitrary sized unsigned integers. | :heavy_check_mark: |
| [math](source/math) | [symbolic](source/math/symbolic) | Compile time symbolic differentiation using template metaprogramming. | :construction: |
| [platform](source/platform) | [cpu](source/platform/cpu) | Class to extract cpuid information to determine supported instructions at runtime. | :heavy_check_mark: |
| [protection](source/protection) | [virtual_machine](source/protection/virtual_machine) | A simple stack based virtual machine allowing easy creation of custom operands. | :heavy_check_mark: |
| [random](source/random) | [random_mt](source/random/random_mt) | Mersenne twister pseudo\-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_pcg](source/random/random_pcg) | PCG pseudo\-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_romu_32](source/random/random_romu_32) | Romu 32 bit pseudo\-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_romu_64](source/random/random_romu_64) | Romu 64 bit pseudo\-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_splitmix](source/random/random_splitmix) | Splitmix pseudo\-random number generator. | :heavy_check_mark: |
| [random](source/random) | [random_xorshift](source/random/random_xorshift) | Xorshift pseudo\-random number generator. | :heavy_check_mark: |
| [string](source/string) | [static_string_encrypted](source/string/static_string_encrypted) | Compile\-time string encryption with runtime decryption to obfuscate strings. | :heavy_check_mark: |
| [type](source/type) | [enum_name](source/type/enum_name) | Compile\-time enum value name as a string with \-fno\-rtti. | :heavy_check_mark: |
| [type](source/type) | [standard](source/type/standard) | Standard type size sanity. | :construction: |
| [type](source/type) | [type_id](source/type/type_id) | Simple run\-time\-type\-information (rtti) with \-fno\-rtti. | :heavy_check_mark: |
| [type](source/type) | [type_name](source/type/type_name) | Compile\-time type name as a string with \-fno\-rtti. | :heavy_check_mark: |
| [utility](source/utility) | [error](source/utility/error) | Composable error class that can be returned down a call stack. | :heavy_check_mark: |
| [utility](source/utility) | [event](source/utility/event) | Thread safe multi\-in/multi\-out event/messaging system. | :heavy_check_mark: |
| [utility](source/utility) | [pimpl](source/utility/pimpl) | Statically sized pointer to implementation wrapper. | :heavy_check_mark: |
| [utility](source/utility) | [sizeof_bitfield](source/utility/sizeof_bitfield) | Macro to get the size in bits of a bitfield. | :construction: |
| [vision](source/vision) | [api](source/vision/api) | An API for a computer vision system that consumes sensor data to perform localisation and mapping. | :construction: |
| [vision/consensus](source/vision/consensus) | [consensus](source/vision/consensus/consensus) | A generic consensus system composed of templated dataset sampler, model estimator, and model evaluator. | :construction: |
| [vision/consensus](source/vision/consensus) | [estimator](source/vision/consensus/estimator) | Interface of a consensus model estimator. | :construction: |
| [vision/consensus/evaluate](source/vision/consensus/evaluate) | [inlier_support](source/vision/consensus/evaluate/inlier_support) | Implementation of a model evaulator that counts the number of inliers under a threshold. | :construction: |
| [vision/consensus/evaluate](source/vision/consensus/evaluate) | [least_median_of_squares](source/vision/consensus/evaluate/least_median_of_squares) | Implementation of a model evaulator that calculates the LMedS score. | :construction: |
| [vision/consensus/evaluate](source/vision/consensus/evaluate) | [maximum_likelihood](source/vision/consensus/evaluate/maximum_likelihood) | Implementation of a model evaulator that calculates a capped residual sum. | :construction: |
| [vision/consensus](source/vision/consensus) | [evaluator](source/vision/consensus/evaluator) | Interface of a consensus model evaluator. | :construction: |
| [vision/consensus/sample](source/vision/consensus/sample) | [exhaustive](source/vision/consensus/sample/exhaustive) | Implementation of an exhaustive dataset sampler. | :construction: |
| [vision/consensus/sample](source/vision/consensus/sample) | [random](source/vision/consensus/sample/random) | Implementation of a random dataset sampler. | :construction: |
| [vision/consensus](source/vision/consensus) | [sampler](source/vision/consensus/sampler) | Interface of a consensus dataset sampler. | :construction: |
| [vision/feature/angle](source/vision/feature/angle) | [orb_angle](source/vision/feature/angle/orb_angle) | Implemetation of feature angle from the paper "ORB: An efficient alternative to SIFT or SURF". IEEE International Conference on Computer Vision (2011). | :construction: |
| [vision/feature](source/vision/feature) | [binary_descriptor](source/vision/feature/binary_descriptor) | Simple structure to hold a buffer of binary data that describes a feature. | :construction: |
| [vision/feature/descriptor](source/vision/feature/descriptor) | [beblid](source/vision/feature/descriptor/beblid) | Implemetation of feature description from the paper "BEBLID: Boosted Efficient Binary Local Image Descriptor". Pattern Recognition Letters (2020). | :construction: |
| [vision/feature/descriptor](source/vision/feature/descriptor) | [orb](source/vision/feature/descriptor/orb) | Implemetation of feature description from the paper "ORB: An efficient alternative to SIFT or SURF". IEEE International Conference on Computer Vision (2011). | :construction: |
| [vision/feature/descriptor](source/vision/feature/descriptor) | [rs_brief](source/vision/feature/descriptor/rs_brief) | Implemetation of feature description from the paper "eSLAM: An Energy\-Efficient Accelerator for Real\-Time ORB\-SLAM on FPGA Platform". Design Automation Conference (2019). | :construction: |
| [vision/feature/detector](source/vision/feature/detector) | [fast](source/vision/feature/detector/fast) | Implemetation of feature detection from the paper "Faster and better: A machine learning approach to corner detection". IEEE transactions on pattern analysis and machine intelligence (2008). | :construction: |
| [vision/feature/distributor](source/vision/feature/distributor) | [square_covering](source/vision/feature/distributor/square_covering) | Implemetation of feature distribution from the paper "Efficient adaptive non\-maximal suppression algorithms for homogeneous spatial keypoint distribution". Pattern Recognition Letters (2018). | :construction: |
| [vision/feature](source/vision/feature) | [feature](source/vision/feature/feature) | Simple structure to hold a the information defining a detected feature. | :construction: |
| [vision/feature/refinement](source/vision/feature/refinement) | [exhaustive](source/vision/feature/refinement/exhaustive) | Exhaustive search method for the best match between two patches. | :construction: |
| [vision/feature/refinement](source/vision/feature/refinement) | [golden_section](source/vision/feature/refinement/golden_section) | Golden section search method for the best match between two patches. | :construction: |
| [vision/feature/refinement](source/vision/feature/refinement) | [quadratic_fitting](source/vision/feature/refinement/quadratic_fitting) | Quadratic fitting search method for the best match between two patches. | :construction: |
| [vision/feature/score](source/vision/feature/score) | [fast_score](source/vision/feature/score/fast_score) | Implemetation of feature quality score from the paper "Faster and better: A machine learning approach to corner detection". IEEE transactions on pattern analysis and machine intelligence (2008). | :construction: |
| [vision/feature/score](source/vision/feature/score) | [harris_score](source/vision/feature/score/harris_score) | Implemetation of feature quality score from the paper "A Combined Corner and Edge Detector". Alvey Vision Conference (1988). | :construction: |
| [vision/feature/score](source/vision/feature/score) | [shi_tomasi_score](source/vision/feature/score/shi_tomasi_score) | Implemetation of feature quality score from the paper "Good Features to Track". IEEE Conference on Computer Vision and Pattern Recognition (1994). | :construction: |
| [vision/image_processing](source/vision/image_processing) | [integral](source/vision/image_processing/integral) | Simple integral image processor. | :construction: |
| [vision/image_processing](source/vision/image_processing) | [sub_pixel_patch](source/vision/image_processing/sub_pixel_patch) | Generation of a sub\-pixel patch for match refinement. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [hamming](source/vision/match_distance/hamming) | Simple hamming matching without the popcount instruction. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [ncc](source/vision/match_distance/ncc) | Normalised cross correleation between two patches. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [sad](source/vision/match_distance/sad) | Sum of absolute distances between two patches. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [ssd](source/vision/match_distance/ssd) | Sum of squared distances between two patches. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [zncc](source/vision/match_distance/zncc) | Zero\-mean normalised cross correleation between two patches. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [zsad](source/vision/match_distance/zsad) | Zero\-mean sum of absolute distances between two patches. | :construction: |
| [vision/match_distance](source/vision/match_distance) | [zssd](source/vision/match_distance/zssd) | Zero\-mean sum of squared distances between two patches. | :construction: |

## License ##

Copyright (C) 2018-2022 Geoffrey Daniels. http://gpdaniels.com/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License only.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

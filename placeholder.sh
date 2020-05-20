#!/bin/bash
set -e

if [ $# -eq 0 ]
then
    echo "No arguments supplied, require type of and name of placeholder."
    exit 1
fi

if [ $# -eq 1 ]
then
    echo "Only one argument supplied, require type of and name of placeholder."
    exit 1
fi

# Ensure we are in the correct directory.
cd $(cd "$(dirname "$0")"; pwd)

TYPE_NAME_LOWER=$(echo "$1" | tr '[:upper:]' '[:lower:]')
TYPE_NAME_UPPER=$(echo "$1" | tr '[:lower:]' '[:upper:]')

CLASS_NAME_LOWER=$(echo "$2" | tr '[:upper:]' '[:lower:]')
CLASS_NAME_UPPER=$(echo "$2" | tr '[:lower:]' '[:upper:]')

mkdir -p Source/${TYPE_NAME_LOWER}

cat << EOF > Source/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}
/*
The MIT License
Copyright (c) 2020 Geoffrey Daniels. http://gpdaniels.com/
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
THE SOFTWARE
*/

#pragma once
#ifndef GTL_${CLASS_NAME_UPPER}_HPP
#define GTL_${CLASS_NAME_UPPER}_HPP

namespace gtl {
    class ${CLASS_NAME_LOWER} {

    };
}

#endif // GTL_${CLASS_NAME_UPPER}_HPP
EOF

mkdir -p Tests/${TYPE_NAME_LOWER}

cat << EOF > Tests/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}.test.cpp
/*
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
THE SOFTWARE
*/

#include <main.tests.hpp>
#include <benchmark.tests.hpp>
#include <require.tests.hpp>

#include <${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}>

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#endif

#include <type_traits>

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

TEST(${CLASS_NAME_LOWER}, traits, standard) {
    REQUIRE(sizeof(gtl::${CLASS_NAME_LOWER}) >= 0);

    REQUIRE((std::is_pod<gtl::${CLASS_NAME_LOWER}>::value == true));

    REQUIRE((std::is_trivial<gtl::${CLASS_NAME_LOWER}>::value == true));

    REQUIRE((std::is_trivially_copyable<gtl::${CLASS_NAME_LOWER}>::value == true));

    REQUIRE((std::is_standard_layout<gtl::${CLASS_NAME_LOWER}>::value == true));
}

TEST(${CLASS_NAME_LOWER}, constructor, empty) {
    gtl::${CLASS_NAME_LOWER} ${CLASS_NAME_LOWER};
    do_not_optimise_away(${CLASS_NAME_LOWER});
}

EOF

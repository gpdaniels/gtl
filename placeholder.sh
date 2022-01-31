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

mkdir -p source/${TYPE_NAME_LOWER}

cat << EOF > source/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}
/*
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
*/

#pragma once
#ifndef GTL_${TYPE_NAME_UPPER}_${CLASS_NAME_UPPER}_HPP
#define GTL_${TYPE_NAME_UPPER}_${CLASS_NAME_UPPER}_HPP

// Summary: [wip]

namespace gtl {
    class ${CLASS_NAME_LOWER} final {

    };
}

#endif // GTL_${CLASS_NAME_UPPER}_HPP
EOF

mkdir -p tests/${TYPE_NAME_LOWER}

cat << EOF > tests/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}.test.cpp
/*
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
*/

#include <main.tests.hpp>
#include <optimise.tests.hpp>
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
    testbench::do_not_optimise_away(${CLASS_NAME_LOWER});
}

EOF

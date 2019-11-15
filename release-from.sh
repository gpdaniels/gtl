#!/bin/bash
set -e

if [ $# -eq 0 ]
  then
    echo "No arguments supplied, require type of and name of class to release, and the source branch."
    exit 1
fi

if [ $# -eq 1 ]
then
    echo "Only one argument supplied, require type of and name of class to release, and the source branch."
    exit 1
fi

if [ $# -eq 2 ]
then
    echo "Only two arguments supplied, require type of and name of class to release, and the source branch."
    exit 1
fi

# Ensure we are in the correct directory.
cd $(cd "$(dirname "$0")"; pwd)

TYPE_NAME_LOWER=$(echo "$1" | tr '[:upper:]' '[:lower:]')
TYPE_NAME_UPPER=$(echo "$1" | tr '[:lower:]' '[:upper:]')

CLASS_NAME_LOWER=$(echo "$2" | tr '[:upper:]' '[:lower:]')
CLASS_NAME_UPPER=$(echo "$2" | tr '[:lower:]' '[:upper:]')

echo "Adding 'Source/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}'"
git checkout $3 Source/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}

echo "Adding 'Tests/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}.test.cpp'"
git checkout $3 Tests/${TYPE_NAME_LOWER}/${CLASS_NAME_LOWER}.test.cpp

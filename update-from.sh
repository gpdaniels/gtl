#!/bin/bash
set -e

if [ $# -eq 0 ]
  then
    echo "No arguments supplied, require target branch and source branch."
    exit 1
fi

if [ $# -eq 1 ]
then
    echo "Only one argument supplied, require target branch and source branch."
    exit 1
fi

# Ensure we are in the correct directory.
cd $(cd "$(dirname "$0")"; pwd)

# Save the current branch.
current_branch=$(git symbolic-ref -q HEAD)
current_branch=${current_branch##refs/heads/}
current_branch=${current_branch:-HEAD}

# Checkout the target branch.
git checkout $1

# Get a list of all files.
source=$(find "Source" -type f -print)
testbench=$(find "Testbench" -type f -print)
tests=$(find "Tests" -type f -print)

# Checkout back to the current branch.
git checkout ${current_branch}

# Try and checkout the CMakesList.txt to ensure the build system is up to date.
git checkout $2 CMakeLists.txt

# Try and checkout all files found in the target branch from the source branch.
for file in ${source}
do
    if [ -f "${file}" ]
    then
        echo "Adding '${file}'"
        git checkout $2 ${file}
    fi
done

for file in ${testbench}
do
    if [ -f "${file}" ]
    then
        echo "Adding '${file}'"
        git checkout $2 ${file}
    fi
done

for file in ${tests}
do
    if [ -f "${file}" ]
    then
        echo "Adding '${file}'"
        git checkout $2 ${file}
    fi
done

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

# Try and checkout the build system files to ensure they are up to date.
git checkout $2 compile.sh
git checkout $2 memcheck.supression
git checkout $2 CMakeLists.txt

# Try and checkout the README.md.
git checkout $2 README.md

# Try and checkout the TODO file.
temp_file=$(mktemp)
if git cat-file -e $2:TODO-BEFORE-FULL-RELEASE.txt > /dev/null 2>&1
then
    git show $2:TODO-BEFORE-FULL-RELEASE.txt > ${temp_file}
fi

function check_todo() {
    if [ -f ${temp_file} ]
    then
        file_name=$(basename $1 .test.cpp)
        if grep -Fxq "${file_name}" "${temp_file}" 
        then
            return 1
        fi
        return 0
    fi
}

# Try and checkout all files found in the target branch from the source branch.
for file in ${source}
do
    if [ -f "${file}" ]
    then
        if check_todo "${file}" $2
        then
            echo "Checking '${file}'..."
            if git cat-file -e $2:${file} > /dev/null 2>&1
            then
                echo "Adding '${file}'."
                git checkout $2 ${file}
            else
                echo "File '${file}' no longer exists on target branch."
            fi
        else
            echo "Ignoring '${file}'."
        fi
    fi
done

for file in ${testbench}
do
    if [ -f "${file}" ]
    then
        if check_todo "${file}" $2
        then
            echo "Checking '${file}'..."
            if git cat-file -e $2:${file} > /dev/null 2>&1
            then
                echo "Adding '${file}'."
                git checkout $2 ${file}
            else
                echo "File '${file}' no longer exists on target branch."
            fi
        else
            echo "Ignoring '${file}'."
        fi
    fi
done

for file in ${tests}
do
    if [ -f "${file}" ]
    then
        if check_todo "${file}" $2
        then
            echo "Checking '${file}'..."
            if git cat-file -e $2:${file} > /dev/null 2>&1
            then
                echo "Adding '${file}'."
                git checkout $2 ${file}
            else
                echo "File '${file}' no longer exists on target branch."
            fi
        else
            echo "Ignoring '${file}'."
        fi
    fi
done

# Remove the temp TODO file
rm ${temp_file}

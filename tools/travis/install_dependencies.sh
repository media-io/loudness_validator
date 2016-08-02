#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e
# Print commands and their arguments as they are executed.
set -x

# Install GTest framework
git clone https://github.com/google/googletest.git
cd googletest
cmake . -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL}
make -k
make install

# Install additional dependencies in case of osx environment
if [ ${TRAVIS_OS_NAME} = "osx" ]; then
    brew update
    brew install scons lcov libsndfile qt
fi

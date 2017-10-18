# Duktape experiment
An excuse to play with [Duktape](http://duktape.org/).

## Quick start
In-source builds are disabled, so create a build directory and invoke CMake as
usual, making sure to define `DUKTAPE_PATH`. For example, to set up a debug
build with `g++`:

    root_dir=`pwd`
    build_dir=build/g++
    mkdir -p "$build_dir"
    cd "$build_dir"
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_COMPILER=g++ \
        -DDUKTAPE_PATH=/path/to/duktape-2.2.0/ \
        "$root_dir"
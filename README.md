# Duktape experiment
An excuse to play with [Duktape](http://duktape.org/).

## Quick start
### Install dependencies
The current dependencies are as follows. In each case, the dependency location
must be passed through to CMake by defining an approprate variable of the form
`<LIBRARY_NAME>_PATH` pointing at the root of that project.
-   [Duktape](https://github.com/svaarala/duktape/) (v2.2.0); define
    `DUKTAPE_PATH`.
-   [easylogging++](https://github.com/muflihun/easyloggingpp) (v9.95.3);
    define `EASYLOGGINGPP_PATH`.

### Build
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
        -DEASYLOGGINGPP_PATH=/path/to/easylogging-9.95.3/ \
        "$root_dir"

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
As usual, only the dependency paths must be defined during CMake configuration.
For example:

    mkdir -p build
    cd build
    cmake \
        -DDUKTAPE_PATH=/path/to/duktape-2.2.0/ \
        -DEASYLOGGINGPP_PATH=/path/to/easylogging-9.95.3/ \
        ..

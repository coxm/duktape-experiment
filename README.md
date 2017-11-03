# Duktape experiment
An excuse to play with [Duktape](http://duktape.org/).

## Quick start
### Install dependencies
The current dependencies are as follows. In each case, the dependency location
must be passed through to CMake by defining an approprate variable of the form
`<LIBRARY_NAME>_PATH` pointing at the root of that project.
-   [Duktape](https://github.com/svaarala/duktape/) (v2.2.0)&mdash;define
    `DUKTAPE_PATH`;
-   [easylogging++](https://github.com/muflihun/easyloggingpp)
    (v9.95.3)&mdash;define `EASYLOGGINGPP_PATH`.
-   [Box2D](http://box2d.org/) (tested with v2.3.2, though other versions will
    probably work);
-   [b2Draw](https://github.com/coxm/b2draw) (tested with v1).

### Build
As usual, except some dependencies are required when configuring CMake. For
example:

    mkdir -p build
    cd build
    cmake \
        -DDUKTAPE_PATH=/path/to/duktape-2.2.0/ \
        -DEASYLOGGINGPP_PATH=/path/to/easylogging-9.95.3/ \
        ..

If other dependencies are not in a regular system location (like `/usr/lib/`,
`/usr/include/`) then you will need to specify their locations too. In such
cases, define `<DEPENDENCY>_INCLUDE_DIR` and/or `<DEPENDENCY>_LIBRARY_DIR` as
appropriate when invoking `cmake`.

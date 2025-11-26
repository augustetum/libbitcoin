# Libbitcoin library 

"The libbitcoin toolkit is a set of cross platform C++ libraries for building bitcoin applications. The toolkit consists of several libraries, most of which depend on the base libbitcoin-system library. Each library's repository can be cloned and built using common automake 1.14+ instructions. There are no packages yet in distribution however each library includes an installation script (described below) which is regularly verified in the automated build."

## Setup on MacOS

### The problems

The setup of the library is quite tricky. The instructions are provided on the libbitcoin's repository, however the library uses a lot of deprecated elements. Thus usual commands like `brew install libbitcoin` do not work. The core issue was enum-constexpr-conversion errors in Boost 1.73.0. Modern Clang enforces strict type checking for enum values, rejecting code that attempts to cast integer value -1 into enums with valid ranges like [0, 3]. This caused compilation failures.

### The solution

1. Install LLVM 14 via homebrew (or however you prefer) by using `brew install llvm@14`
2. Set compiler env
````
export CC=/opt/homebrew/opt/llvm@15/bin/clang
export CXX=/opt/homebrew/opt/llvm@15/bin/clang++
````
3. Configure build flags
````
export BOOST_CXXFLAGS="cxxflags=-Wno-enum-constexpr-conversion"
export CXXFLAGS="-Wno-enum-constexpr-conversion"
````
4. Run the installation
````
//Change directory name accordingly
./install.sh --prefix=$HOME/libbitcoin-installation --build-boost --disable-shared
````

### Usage in a project

1. Tell the compiler where to find libbitcoin
````
//Change directory name accordingly
export PKG_CONFIG_PATH=$HOME/libbitcoin-installation/lib/pkgconfig:$PKG_CONFIG_PATH
export CPPFLAGS="-I$HOME/libbitcoin-installation/include"
export LDFLAGS="-L$HOME/libbitcoin-installation/lib"
````

2. Compile the project with a warning suppression flag (as there are a LOT of warnings due to deprecated elements):
````
clang++ -std=c++11 -Wno-enum-constexpr-conversion -o merkle merkle.cpp $(pkg-config --cflags --libs libbitcoin-system)
````

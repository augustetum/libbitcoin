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

If something like this is seen, nothing to worry about, this is the normal amount of warnings:

````
... earlier warnings
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__functional/unary_function.h:23:29: note: 'unary_function<const std::error_category *, unsigned long>' has been explicitly marked deprecated here
   23 | struct _LIBCPP_TEMPLATE_VIS _LIBCPP_DEPRECATED_IN_CXX11 unary_function {
      |                             ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__config:719:41: note: expanded from macro '_LIBCPP_DEPRECATED_IN_CXX11'
  719 | #    define _LIBCPP_DEPRECATED_IN_CXX11 _LIBCPP_DEPRECATED
      |                                         ^
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1/__config:696:49: note: expanded from macro '_LIBCPP_DEPRECATED'
  696 | #      define _LIBCPP_DEPRECATED __attribute__((__deprecated__))
      |                                                 ^
42 warnings generated.
````

Taking the transactions from a block (925284), I tested the libbitcoin with the provided code. This was the output.

````
Current merkle hash list:
 876a9829dca8066f740dfae65088146d3b1be37f48c92dabab8b88e8dad55ae0
 703bb4fee0ca97ba4ded46824aa8fd694e9a1453b0881a9854fa7d8809d8e370
 898d561abd8dced57a2b963546f82efe4c63fa6cd94073cede2a1cabf93b61cd
 086dfd62b14e0b22dc573c9e73437cdd3fc144abc8a2d43f18395fe5c023d555
 2b8a6c25168e1845a7b508b69d95102d079ca14dfe1472d96457b217b52543b7
 69c57818951a3b5b7e2b1e8cef126b780b5c35948f97f766f8ccc6e896349dbf
 d8a537ddadbe5d180996a2be5e9c5909b7c53c35db055918f440c4b5ce14a85f
 23a99500b99fe60b441b670cee6d4f34a2a8393c4b60a5db96be8439884e851f

Current merkle hash list:
 d87f36a861c4ad43c3b5a8f483efa0a72cf503a0641a87c543e42f5548007f2a
 53ec91aa8b355801572db991313dcbd72e7b62998b7f46d01022f30e551178df
 fe7c011eb5f6521e844cc94e8d9cfc7c2626285b61400e8bb26b58ae95cb9a16
 d9b84d383a6e4898cafec305c152d9071de97c7fdfcd4eac23e3949b34d0aecd

Current merkle hash list:
 269695d826ee3fa91de8e6f2d0d5a60c8ba2783835a2d0868f78240f7c40648a
 37a96a9eabcb4f1d206f45758c288cc6761641024bb5337f52e69ba8820c23a0

Current merkle hash list:
 84f7ed5b2aa788e62a74e0f15b022bbe7d4de17229df8b6e34dbf8f066e4f792

Merkle Root Hash: 84f7ed5b2aa788e62a74e0f15b022bbe7d4de17229df8b6e34dbf8f066e4f792
````

### Integration with LIDL hash
To integrate libbitcoin with LIDL hash, I simply added the provided code to a class called `libbitcoinMerkle.cpp`. The most important part of that class is the converter to libbitcoin hash_digest as our hash previously just used a vector of strings. It then converts the merkle root BACK to a string when the program is finished.

The converter is here:
````
std::string LibBitcoinMerkle::computeMerkleRoot(const std::vector<std::string> &data)
{
    //converts to libbitcoin format
    bc::hash_list merkleList;
    for (const auto &hexStr : data)
    {
        merkleList.push_back(stringToHashDigest(hexStr));
    }

    bc::hash_digest merkleRoot = create_merkle(merkleList);

    return hashDigestToString(merkleRoot);
}
````

You then need to use this long (and annoying) compile command:

````
clang++ -std=c++11 -Wno-enum-constexpr-conversion -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -o program Program.cpp Block.cpp BlockHeader.cpp Transaction.cpp libbitcoinMerkle.cpp customGenerator.cpp Functions.cpp -lomp $(pkg-config --cflags --libs libbitcoin-system)
````

And the code runs as expected. Both the libbitcoin and old code produce the same hash:

````
//libbitcoin
Block 3
Block Hash: 000057b72ddb1a8f875d5bc885afbdee16f75635ea4b403f80fbaf48567ce336
Previous Hash: 000035e21186d0451ce57ef462b582e29c8a66f06902a53018b57ebea7802886
Merkle Root: 67a6be6d96915444ae856c1c9c1f1eeea5a0282e6e92cf344ed3495f829b85f9
Timestamp: 1766046951
Difficulty: 4
Nonce: 18159
Transactions: 100

//old code (our merkle root calculator)
Block 3
Block Hash: 000057b72ddb1a8f875d5bc885afbdee16f75635ea4b403f80fbaf48567ce336
Previous Hash: 000035e21186d0451ce57ef462b582e29c8a66f06902a53018b57ebea7802886
Merkle Root: 67a6be6d96915444ae856c1c9c1f1eeea5a0282e6e92cf344ed3495f829b85f9
Timestamp: 1766046951
Difficulty: 4
Nonce: 18159
Transactions: 100
````
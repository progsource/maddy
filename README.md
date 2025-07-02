# maddy

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version: 1.5.0](https://img.shields.io/badge/Version-1.5.0-brightgreen.svg)](https://semver.org/) <!-- MADDY_VERSION_LINE_REPLACEMENT -->

maddy is a C++ Markdown to HTML **header-only** parser library.

## Supported OS

It actually should work on any OS, that supports the C++14 standard library.

It is tested to work on:

* Linux (gcc)
* OSX (clang)
* Windows (Visual Studio 17 2022, mingw)

## Dependencies

* C++14

## Why maddy?

When I was looking for a Markdown parser in C++, I couldn't find any, that was
fitting my needs. So I simply wrote my own one.

## Markdown syntax

The supported syntax can be found in the [definitions docs](docs/definitions.md).

## How to add maddy to your cmake project

Use `find_package()` which provides `maddy::maddy` target:

```cmake
find_package(maddy REQUIRED)

add_executable(my_exe)
target_link_libraries(my_exe PRIVATE maddy::maddy)
```

Or you can use [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)
which was introduced in CMake 3.11.

This way you can add

```cmake
include(FetchContent)

FetchContent_Declare(
  maddy
  URL https://github.com/progsource/maddy/.../maddy-src.zip
)
FetchContent_MakeAvailable(maddy)

add_executable(my_exe)
target_link_libraries(my_exe PUBLIC maddy)
```

to your CMake file to make it work. Check the
[release](https://github.com/progsource/maddy/releases) for the full
zip-file-url.

The zip only contains a `CMakeLists.txt`, the `include` folder and the `LICENSE`
file.

## How to use

To use maddy in your project, simply add the include path of maddy to yours
and in the code, you can then do the following:

```c++
#include <memory>
#include <string>

#include "maddy/parser.h"

std::stringstream markdownInput("");

// config is optional
std::shared_ptr<maddy::ParserConfig> config = std::make_shared<maddy::ParserConfig>();
config->enabledParsers &= ~maddy::types::EMPHASIZED_PARSER; // disable emphasized parser
config->enabledParsers |= maddy::types::HTML_PARSER; // do not wrap HTML in paragraph

std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>(config);
std::string htmlOutput = parser->Parse(markdownInput);
```

You can find all parser flags in
[`include/maddy/parserconfig.h`](include/maddy/parserconfig.h).

## How to run the tests

*(tested on Linux with
[git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) and
[cmake](https://cmake.org/install/) installed)*

Open your preferred terminal and type:

```shell
git clone https://github.com/progsource/maddy.git
cd maddy
mkdir tmp
cd tmp
cmake -DMADDY_BUILD_WITH_TESTS=ON ..
make
make test # or run the executable in ../build/MaddyTests
```

## How to run the benchmarks

To get proper test results, the benchmarks should always be compiled as
release build.

```shell
git clone https://github.com/progsource/maddy.git
cd maddy
mkdir tmp
cd tmp
cmake -DMADDY_BUILD_WITH_BENCH=ON -DCMAKE_BUILD_TYPE=Release ..
make BUILD_TYPE=Release
../build/maddy_benchmark
```

## How to contribute

There are different possibilities:

* [Create a GitHub issue](https://github.com/progsource/maddy/issues/new)
* Create a pull request with an own branch

Please also read [CONTRIBUTING.md](CONTRIBUTING.md).

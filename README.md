# maddy

[![Join the chat at https://gitter.im/progsource-maddy/Lobby](https://badges.gitter.im/progsource-maddy/Lobby.svg)](https://gitter.im/progsource-maddy/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version: 1.0.3](https://img.shields.io/badge/Version-1.0.3-brightgreen.svg)](https://semver.org/)
[![Build Status](https://travis-ci.org/progsource/maddy.svg?branch=master)](https://travis-ci.org/progsource/maddy)

maddy is a C++ Markdown to HTML **header-only** parser library.

## Supported OS

It actually should work on any OS, that supports the C++14 standard library.

It is tested to work on:

* Linux (without exceptions and without RTTI)

## Dependencies

* C++14

## Why maddy?

When I was needing a Markdown parser in C++ I couldn't find any, that was
fitting my needs. So I simply wrote my own one.

## Markdown syntax

The supported syntax can be found in the [definitions docs](docs/definitions.md).

## How to use

To use maddy in your project, simply add the include path of maddy to yours
and in the code, you can then do the following:

```c++
#include <memory>
#include <string>

#include "maddy/parser.h"

std::stringstream markdownInput("");
std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
std::string htmlOutput = parser->Parse(markdownInput);
```

## How to run the tests

*(tested on Linux with
[git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git) and
[cmake](https://cmake.org/install/) installed)*

Open your preferred terminal and type:

```shell
git clone https://github.com/progsource/maddy.git
cd maddy
git submodule update --init --recursive
mkdir tmp
cd tmp
cmake ..
make
make test # or run the executable in ../build/MaddyTests
```

## How to contribute

There are different possibilities:

* Create a GitHub issue
* Create a pull request with an own branch (don't forget to put yourself in the
  AUTHORS file)

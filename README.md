# maddy

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version: 1.0.2](https://img.shields.io/badge/Version-1.0.2-brightgreen.svg)](https://semver.org/)

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

## HowTo use

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

## How to contribute

There are different possibilities:

* Create a GitHub issue
* Create a pull request with an own branch (don't forget to put yourself in the
  AUTHORS file)

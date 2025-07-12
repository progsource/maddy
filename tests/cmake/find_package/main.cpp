/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 *
 * This file is a tiny example project to test if find_package works correctly.
 */
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "maddy/parser.h"

int main(int argc, char** argv)
{
  std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();

  std::stringstream markdownStream;
  markdownStream << "# Hello World\n"
                 << "This is a **bold** text and this is *italic* text.\n";

  std::cout << parser->Parse(markdownStream) << std::endl;

  return 0;
}

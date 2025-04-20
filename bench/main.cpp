/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include "maddy/parser.h"

int main()
{
  static const std::string markdownFile =
    std::string(CURRENT_FILE_PATH) + "/benchmark_test.md";
  std::stringstream buffer;

  {
    std::ifstream file(markdownFile);

    if (!file.good() || !file.is_open())
    {
      std::cout << "could not read file at " << markdownFile << std::endl;
      return 1;
    }

    buffer << file.rdbuf();

    file.close();
  }

  if (!buffer.good() || buffer.str().empty())
  {
    std::cout << "buffer is invalid" << std::endl;
    return 2;
  }

  // maddy 1.*
  std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();

  // This is the place in the future to compare maddy with other libraries.
  // For now it can be used to check if changes in maddy code result in better
  // performance.

  ankerl::nanobench::Bench()
    .title("maddy test")
    .warmup(100)
    .relative(true)
    .run(
      "maddy 1.x",
      [&]()
      {
        buffer.clear(); // clear any error flags
        buffer.seekg(0, buffer.beg);
        ankerl::nanobench::doNotOptimizeAway(parser->Parse(buffer));
      }
    );

  return 0;
}

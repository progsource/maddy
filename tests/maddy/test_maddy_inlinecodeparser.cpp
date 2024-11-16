/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/inlinecodeparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_INLINECODEPARSER, ItReplacesMarkdownWithCodeHTML)
{
  std::string text = "some text `bla` text testing `it` out";
  std::string expected =
    "some text <code>bla</code> text testing <code>it</code> out";
  auto emphasizedParser = std::make_shared<maddy::InlineCodeParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

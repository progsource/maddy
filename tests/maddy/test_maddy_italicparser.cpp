/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/italicparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_ITALICPARSER, ItReplacesMarkdownWithItalicHTML)
{
  std::string text = "some text *bla* text testing *it* out";
  std::string expected = "some text <i>bla</i> text testing <i>it</i> out";
  auto italicParser = std::make_shared<maddy::ItalicParser>();

  italicParser->Parse(text);

  ASSERT_EQ(text, expected);
}

/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/strikethroughparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_STRIKETHROUGHPARSER, ItReplacesMarkdownWithStrikeThroughHTML)
{
  std::string text = "some text ~~bla~~ text testing ~~it~~ out";
  std::string expected = "some text <s>bla</s> text testing <s>it</s> out";
  auto strikeThroughParser = std::make_shared<maddy::StrikeThroughParser>();

  strikeThroughParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRIKETHROUGHPARSER, ItDoesNotParseInsideInlineCode)
{
  std::string text =
    "some text `~~bla~~` ` ~~text~~ ` testing <code>~~it~~</code> out";
  std::string expected =
    "some text `~~bla~~` ` ~~text~~ ` testing <code>~~it~~</code> out";
  auto strikeThroughParser = std::make_shared<maddy::StrikeThroughParser>();

  strikeThroughParser->Parse(text);

  ASSERT_EQ(expected, text);
}

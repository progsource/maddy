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

TEST(MADDY_INLINECODEPARSER, ItProperlyEscapesHTML)
{
  std::string text =
    "some text `<h1>Test</h1>` text testing `<span>it</span>` out";
  std::string expected =
    "some text <code>&lt;h1&gt;Test&lt;/h1&gt;</code> text testing "
    "<code>&lt;span&gt;it&lt;/span&gt;</code> out";
  auto emphasizedParser = std::make_shared<maddy::InlineCodeParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

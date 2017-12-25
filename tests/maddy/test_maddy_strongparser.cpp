/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/strongparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_STRONGPARSER, ItReplacesMarkdownWithStrongHTML)
{
  std::string text = "some text **bla** text testing **it** out";
  std::string expected = "some text <strong>bla</strong> text testing <strong>it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItReplacesEmphasizedMarkdownNotWithStrongHTML)
{
  std::string text = "some text *bla* text testing **it** out";
  std::string expected = "some text *bla* text testing <strong>it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItDoesNotParseInsideInlineCode)
{
  std::string text = "some text *bla* text testing `**it**` out";
  std::string expected = "some text *bla* text testing `**it**` out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

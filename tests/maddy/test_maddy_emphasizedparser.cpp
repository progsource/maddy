/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/emphasizedparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_EMPHASIZEDPARSER, ItReplacesMarkdownWithEmphasizedHTML)
{
  std::string text = "some text _bla_ text testing _it_ out";
  std::string expected = "some text <em>bla</em> text testing <em>it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItDoesNotParseInsideInlineCode)
{
  std::string text = "some text `*bla*` `/**text*/` testing _it_ out";
  std::string expected =
    "some text `*bla*` `/**text*/` testing <em>it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

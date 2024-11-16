/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/linkparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithALink)
{
  std::string text = "Some text [Link Title](http://example.com)";
  std::string expected =
    "Some text <a href=\"http://example.com\">Link Title</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithLinks)
{
  std::string text =
    "Some text [Link Title](http://example.com) bla [Link "
    "Title](http://example.com)";
  std::string expected =
    "Some text <a href=\"http://example.com\">Link Title</a> bla <a "
    "href=\"http://example.com\">Link Title</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

// -----------------------------------------------------------------------------

class DISABLED_MADDY_LINKPARSER : public ::testing::Test
{};

// This test is disabled for now, so make sure, to first run the ImageParser
// before using the LinkParser
TEST_F(DISABLED_MADDY_LINKPARSER, ItReplacesNoImageMarkdownWithLinks)
{
  std::string text = "Some text ![Image Title](http://example.com/example.png)";
  std::string expected(text);
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

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

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithSpacesAfterLink)
{
  std::string text =
    "Some text [Link Title](http://example.com    ) bla [Link "
    "Title](http://example.com)";
  std::string expected =
    "Some text <a href=\"http://example.com\">Link Title</a> bla <a "
    "href=\"http://example.com\">Link Title</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItHandlesURLsWithOfficiallyIllegalCharacters)
{
  // Some links in the real world have characters that are not
  // 'official' characters that are supposedly allowed in URLs.
  std::string text =
    "Wikipedia's [Möbius strip](https://en.wikipedia.org/wiki/"
    "Möbius_strip) link.";
  std::string expected =
    "Wikipedia's <a href=\"https://en.wikipedia.org/wiki/"
    "Möbius_strip\">Möbius strip</a> link.";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(
  MADDY_LINKPARSER, ItReplacesMarkdownProperlyEvenWithMultipleParenthesisInLine
)
{
  std::string text =
    "(This is a [link](/ABC/some_file) (the URL will not include this).)";
  std::string expected =
    "(This is a <a href=\"/ABC/some_file\">link</a> (the URL will not include "
    "this).)";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItDoesntReplaceMarkdownWithSpaceInURL)
{
  // Spaces are not allowed in URLs, so don't match them.
  std::string text = "This is an invalid [link](/ABC/some file)";
  std::string expected = "This is an invalid [link](/ABC/some file)";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithTitleText)
{
  std::string text = "Link to [name](http:://example.com \"title text\")";
  std::string expected =
    "Link to <a href=\"http:://example.com\" title=\"title text\">name</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithSpacesWithTitleText)
{
  std::string text = "Link to [name](http:://example.com     \"title text\")";
  std::string expected =
    "Link to <a href=\"http:://example.com\" title=\"title text\">name</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithMoreSpacesWithTitleText)
{
  std::string text =
    "Link to [name](http:://example.com     \"title text\"    )";
  std::string expected =
    "Link to <a href=\"http:://example.com\" title=\"title text\">name</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItReplacesMarkdownWithParentheticalText)
{
  std::string text = "Link to [name](http:://example.com \"title (text)\")";
  std::string expected =
    "Link to <a href=\"http:://example.com\" title=\"title (text)\">name</a>";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItDoesntReplaceMarkdownWithTooManyQuotes)
{
  // If you have too many quotation marks, don't match:
  std::string text =
    "This is an invalid [link](/ABC/some_file \"title \" text \")";
  std::string expected =
    "This is an invalid [link](/ABC/some_file \"title \" text \")";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_LINKPARSER, ItDoesntReplaceMarkdownWithQuoteInLink)
{
  // This is actually legal markdown, but hard to parse with regexes;
  // See disabled 'ItReplacesMarkdownWithQuoteInLink' below.
  //
  // For now, don't try to translate it; it would produce invalid HTML.

  std::string text = "Some text [Link Title](http://example.com/\"foo ) bla.";
  std::string current_expected =
    "Some text [Link Title](http://example.com/\"foo ) bla.";
  std::string correct_expected =
    "Some text <a href=\"http://example.com/%22foo\">Link Title</a> bla.";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(current_expected, text);
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

TEST(DISABLED_MADDY_LINKPARSER, ItReplacesMarkdownWithQuoteInLink)
{
  // This is legal markdown, but hard to parse with regexes; dropping it
  // here for a future update.
  std::string text = "Some text [Link Title](http://example.com/\"foo ) bla.";
  std::string expected =
    "Some text <a href=\"http://example.com/%22foo\">Link Title</a> bla.";
  auto linkParser = std::make_shared<maddy::LinkParser>();

  linkParser->Parse(text);

  ASSERT_EQ(expected, text);
}

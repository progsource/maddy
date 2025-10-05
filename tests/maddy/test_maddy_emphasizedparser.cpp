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

TEST(MADDY_EMPHASIZEDPARSER, ItReplacesUnderscoresAtStringEdges)
{
  std::string text = "_some text_";
  std::string expected = "<em>some text</em>";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItDoesNotReplaceMarkdownWithInlineUnderscores)
{
  std::string text = "some text_bla_text testing _it_ out";
  std::string expected = "some text_bla_text testing <em>it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItOnlyReplacesUnderscoresAtWordBreaks)
{
  std::string text = "some _text_bla_ testing _it_ out";
  std::string expected = "some <em>text_bla</em> testing <em>it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItReplacesUnderscoresWithMultipleWords)
{
  std::string text = "some _text testing it_ out";
  std::string expected = "some <em>text testing it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItAllowsDoubleUnderscores)
{
  // I'm not sure if this is standard or not, but this is how the github
  // markdown parser behaves.  Other things I've seen want it to *not*
  // match.
  std::string text = "some __text testing it_ out";
  std::string expected = "some <em>_text testing it</em> out";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItDoesntReplaceUnderscoresInsideCodeBlocks)
{
  std::string text =
    "Stuff inside <code> blocks _shouldn't be emphasized_ </code> at all";
  std::string expected =
    "Stuff inside <code> blocks _shouldn't be emphasized_ </code> at all";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItDoesNotReplaceUnderscoresInURLs)
{
  std::string text = "[Link Title](http://example.com/what_you_didn't_know)";
  std::string expected =
    "[Link Title](http://example.com/what_you_didn't_know)";
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

TEST(MADDY_EMPHASIZEDPARSER, ItParsesOutsideCodeBlocks)
{
  std::string text =
    "Stuff inside <code> blocks _shouldn't be emphasized_ </code>"
    " but outside _should_.";
  std::string expected =
    "Stuff inside <code> blocks _shouldn't be emphasized_ </code>"
    " but outside <em>should</em>.";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_EMPHASIZEDPARSER, ItParsesOutsideTickBlocks)
{
  std::string text =
    "Stuff inside `blocks _shouldn't be emphasized_ `"
    " but outside _should_.";
  std::string expected =
    "Stuff inside `blocks _shouldn't be emphasized_ `"
    " but outside <em>should</em>.";
  auto emphasizedParser = std::make_shared<maddy::EmphasizedParser>();

  emphasizedParser->Parse(text);

  ASSERT_EQ(expected, text);
}

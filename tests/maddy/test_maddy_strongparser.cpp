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
  struct testIt
  {
    std::string text;
    std::string expected;
  };

  std::vector<testIt> tests{
    {"some text **bla** text testing **it** out",
     "some text <strong>bla</strong> text testing <strong>it</strong> out"},
    {"some text __bla__ text testing __it__ out",
     "some text <strong>bla</strong> text testing <strong>it</strong> out"},
  };

  auto strongParser = std::make_shared<maddy::StrongParser>();

  for (auto& test : tests)
  {
    strongParser->Parse(test.text);
    ASSERT_EQ(test.expected, test.text);
  }
}

TEST(MADDY_STRONGPARSER, ItReplacesEmphasizedMarkdownNotWithStrongHTML)
{
  struct testIt
  {
    std::string text;
    std::string expected;
  };

  std::vector<testIt> tests{
    {"some text *bla* text testing **it** out",
     "some text *bla* text testing <strong>it</strong> out"},
    {"some text _bla_ text testing __it__ out",
     "some text _bla_ text testing <strong>it</strong> out"},
  };

  auto strongParser = std::make_shared<maddy::StrongParser>();

  for (auto& test : tests)
  {
    strongParser->Parse(test.text);
    ASSERT_EQ(test.expected, test.text);
  }
}

TEST(MADDY_STRONGPARSER, ItDoesNotParseInsideInlineCode)
{
  struct testIt
  {
    std::string text;
    std::string expected;
  };

  std::vector<testIt> tests{
    {
      "some text **bla** `/**text**/` testing `**it**` out",
      "some text **bla** `/**text**/` testing `**it**` out",
    },
    {"some text _bla_ text testing __it__ out",
     "some text _bla_ text testing <strong>it</strong> out"},
  };

  auto strongParser = std::make_shared<maddy::StrongParser>();

  for (auto& test : tests)
  {
    strongParser->Parse(test.text);
    ASSERT_EQ(test.expected, test.text);
  }
}

TEST(MADDY_STRONGPARSER, ItReplacesUnderscoresAtStringEdges)
{
  std::string text = "__some text__";
  std::string expected = "<strong>some text</strong>";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(DISABLED_MADDY_STRONGPARSER, ItDoesNotReplaceMarkdownWithInlineUnderscores)
{
  std::string text = "some text__bla__text testing __it__ out";
  std::string expected = "some text__bla__text testing <strong>it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(DISABLED_MADDY_STRONGPARSER, ItOnlyReplacesUnderscoresAtWordBreaks)
{
  std::string text = "some __text__bla__ testing __it__ out";
  std::string expected =
    "some <strong>text__bla</strong> testing <strong>it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItReplacesUnderscoresWithMultipleWords)
{
  std::string text = "some __text testing it__ out";
  std::string expected = "some <strong>text testing it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(DISABLED_MADDY_STRONGPARSER, ItAllowsTripleUnderscores)
{
  // I'm not sure if this is standard or not, but this is how the github
  // markdown parser behaves.  Other things I've seen want it to *not*
  // match.

  std::string text = "some ___text testing it__ out";
  std::string expected = "some <strong>_text testing it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItDoesntReplaceUnderscoresInsideCodeBlocks)
{
  std::string text =
    "Stuff inside <code> blocks __shouldn't be strong__ </code> at all";
  std::string expected =
    "Stuff inside <code> blocks __shouldn't be strong__ </code> at all";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(DISABLED_MADDY_STRONGPARSER, ItDoesNotReplaceUnderscoresInURLs)
{
  std::string text = "[Link Title](http://example.com/what__you__didn't__know)";
  std::string expected =
    "[Link Title](http://example.com/what__you__didn't__know)";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItParsesOutsideCodeBlocks)
{
  std::string text =
    "Stuff inside <code> blocks __shouldn't be strong__ </code>"
    " but outside __should__.";
  std::string expected =
    "Stuff inside <code> blocks __shouldn't be strong__ </code>"
    " but outside <strong>should</strong>.";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItParsesOutsideTickBlocks)
{
  std::string text =
    "Stuff inside `blocks __shouldn't be strong__ `"
    " but outside __should__.";
  std::string expected =
    "Stuff inside `blocks __shouldn't be strong__ `"
    " but outside <strong>should</strong>.";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

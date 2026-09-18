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
      // Per CommonMark, a code span protects only its own extent: text
      // before it (here, "**bla**") is still eligible for parsing.
      "some text **bla** `/**text**/` testing `**it**` out",
      "some text <strong>bla</strong> `/**text**/` testing `**it**` out",
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

// The following cases are adapted from the CommonMark spec
// (https://spec.commonmark.org/), which defines how a code span's
// backtick delimiters are matched and how it interacts with surrounding
// markup.

TEST(MADDY_STRONGPARSER, ItMatchesBacktickRunsByEqualLength)
{
  // CommonMark spec example 349: "`foo``bar``" -> "`foo<code>bar</code>".
  // The lone opening backtick has no closing run of length 1 (the next
  // runs are length 2), so it is ordinary text; the two length-2 runs
  // pair up into the code span. Bold text on either side of this is
  // still parsed normally.
  std::string text = "**pre** `foo``bar`` **post**";
  std::string expected =
    "<strong>pre</strong> `foo``bar`` <strong>post</strong>";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItLetsALongerBacktickFenceProtectAnInnerBacktick)
{
  // CommonMark spec example 329: "`` foo ` bar ``" -> "<code>foo ` bar</code>".
  // A double-backtick fence spans across a single backtick in its
  // content; text after the fence is still parsed normally.
  std::string text = "`` foo ` bar `` and **bold**";
  std::string expected = "`` foo ` bar `` and <strong>bold</strong>";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItReplacesUnderscoresAtStringEdges)
{
  std::string text = "__some text__";
  std::string expected = "<strong>some text</strong>";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItDoesNotReplaceMarkdownWithInlineUnderscores)
{
  std::string text = "some text__bla__text testing __it__ out";
  std::string expected = "some text__bla__text testing <strong>it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItOnlyReplacesUnderscoresAtWordBreaks)
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

TEST(MADDY_STRONGPARSER, ItAllowsTripleUnderscores)
{
  // Per CommonMark, a leftover delimiter from an unbalanced run renders
  // outside the tag it didn't pair into, not inside it.
  std::string text = "some ___text testing it__ out";
  std::string expected = "some _<strong>text testing it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItAllowsTrailingTripleUnderscores)
{
  std::string text = "some __text testing it___ out";
  std::string expected = "some <strong>text testing it</strong>_ out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItAllowsManyLeadingLeftoverUnderscores)
{
  std::string text = "some ________text testing it__ out";
  std::string expected = "some ______<strong>text testing it</strong> out";
  auto strongParser = std::make_shared<maddy::StrongParser>();

  strongParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_STRONGPARSER, ItAllowsManyTrailingLeftoverUnderscores)
{
  std::string text = "some __text testing it_______ out";
  std::string expected = "some <strong>text testing it</strong>_____ out";
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

TEST(MADDY_STRONGPARSER, ItDoesNotReplaceUnderscoresInURLs)
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

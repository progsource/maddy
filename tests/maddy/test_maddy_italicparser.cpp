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

// The following cases are adapted from the CommonMark spec
// (https://spec.commonmark.org/), which defines how a code span's
// backtick delimiters are matched and how it interacts with surrounding
// markup.

TEST(MADDY_ITALICPARSER, ItMatchesBacktickRunsByEqualLength)
{
  // CommonMark spec example 349: "`foo``bar``" -> "`foo<code>bar</code>".
  // The lone opening backtick has no closing run of length 1 (the next
  // runs are length 2), so it is ordinary text; the two length-2 runs
  // pair up into the code span. Italic text on either side of this is
  // still parsed normally.
  std::string text = "*pre* `foo``bar`` *post*";
  std::string expected = "<i>pre</i> `foo``bar`` <i>post</i>";
  auto italicParser = std::make_shared<maddy::ItalicParser>();

  italicParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_ITALICPARSER, ItLetsALongerBacktickFenceProtectAnInnerBacktick)
{
  // CommonMark spec example 329: "`` foo ` bar ``" -> "<code>foo ` bar</code>".
  // A double-backtick fence spans across a single backtick in its
  // content; text after the fence is still parsed normally.
  std::string text = "`` foo ` bar `` and *italic*";
  std::string expected = "`` foo ` bar `` and <i>italic</i>";
  auto italicParser = std::make_shared<maddy::ItalicParser>();

  italicParser->Parse(text);

  ASSERT_EQ(expected, text);
}

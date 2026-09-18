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

// The following cases are adapted from the CommonMark spec
// (https://spec.commonmark.org/), which defines how a code span's
// backtick delimiters are matched and how it interacts with surrounding
// markup.

TEST(MADDY_STRIKETHROUGHPARSER, ItMatchesBacktickRunsByEqualLength)
{
  // CommonMark spec example 349: "`foo``bar``" -> "`foo<code>bar</code>".
  // The lone opening backtick has no closing run of length 1 (the next
  // runs are length 2), so it is ordinary text; the two length-2 runs
  // pair up into the code span. Struck-through text on either side of
  // this is still parsed normally.
  std::string text = "~~pre~~ `foo``bar`` ~~post~~";
  std::string expected = "<s>pre</s> `foo``bar`` <s>post</s>";
  auto strikeThroughParser = std::make_shared<maddy::StrikeThroughParser>();

  strikeThroughParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(
  MADDY_STRIKETHROUGHPARSER, ItLetsALongerBacktickFenceProtectAnInnerBacktick
)
{
  // CommonMark spec example 329: "`` foo ` bar ``" -> "<code>foo ` bar</code>".
  // A double-backtick fence spans across a single backtick in its
  // content; text after the fence is still parsed normally.
  std::string text = "`` foo ` bar `` and ~~struck~~";
  std::string expected = "`` foo ` bar `` and <s>struck</s>";
  auto strikeThroughParser = std::make_shared<maddy::StrikeThroughParser>();

  strikeThroughParser->Parse(text);

  ASSERT_EQ(expected, text);
}

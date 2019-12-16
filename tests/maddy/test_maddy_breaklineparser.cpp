/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/breaklineparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_BREAKLINEPARSER, ItReplacesMarkdownWithBreakLineHTML)
{
  std::string text =
    "Test the text\r\n"
    "test text to check\r\n"
    "check testing to text.\r"
    "Check test to test text\r"
    "This is a test\r\n"
    "No more test to check";

  std::string expected =
    "Test the text<br>"
    "test text to check<br>"
    "check testing to text.<br>"
    "Check test to test text<br>"
    "This is a test<br>"
    "No more test to check";
  auto breakLineParser = std::make_shared<maddy::BreakLineParser>();

  breakLineParser->Parse(text);

  ASSERT_EQ(text, expected);
}

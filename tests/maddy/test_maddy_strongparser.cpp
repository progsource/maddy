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

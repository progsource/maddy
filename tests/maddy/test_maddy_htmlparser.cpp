/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/htmlparser.h"

// -----------------------------------------------------------------------------

class MADDY_HTMLPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::HtmlParser> pParser;

  void SetUp() override
  {
    this->pParser = std::make_shared<maddy::HtmlParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(MADDY_HTMLPARSER, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(pParser->IsFinished());
}

TEST_F(MADDY_HTMLPARSER, IsStartingLineReturnsFalseWhenFacedWithNoSmallerThan)
{
  const std::vector<std::string> markdown = {
    "> quote", "some text", "* list", "1. numbered list", "|table>"
  };

  for (size_t i = 0; i < markdown.size(); ++i)
  {
    ASSERT_FALSE(maddy::HtmlParser::IsStartingLine(markdown[i]));
  }
}

TEST_F(MADDY_HTMLPARSER, IsStartingLineReturnsTrueWhenFacedWithSmallerThan)
{
  const std::string markdown = "<div id=\"test\">test element</div>";

  ASSERT_TRUE(maddy::HtmlParser::IsStartingLine(markdown));
}

TEST_F(MADDY_HTMLPARSER, ItReplacesNoHtml)
{
  const std::vector<std::string> markdown{
    "some text in a paragraph",
    "",
    "<div> some HTML</div>",
    "",
    "<div>more",
    "HTML",
    "</div>",
    ""
  };
  const std::string expected =
    "some text in a paragraph <div> some HTML</div><div>more HTML </div>";

  for (std::string md : markdown)
  {
    pParser->AddLine(md);
  }
  ASSERT_TRUE(pParser->IsFinished());

  std::stringstream& output(pParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

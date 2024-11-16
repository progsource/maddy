/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/headlineparser.h"

// -----------------------------------------------------------------------------

class MADDY_HEADLINEPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::HeadlineParser> hlParser;

  void SetUp() override
  {
    this->hlParser = std::make_shared<maddy::HeadlineParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_HEADLINEPARSER, IsStartingLineReturnsTrueWhenFacedWithOneToSixHashes
)
{
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("# a"));
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("## a"));
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("### a"));
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("#### a"));
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("##### a"));
  ASSERT_TRUE(maddy::HeadlineParser::IsStartingLine("###### a"));
}

TEST_F(MADDY_HEADLINEPARSER, IsFinishedAlwaysReturnsTrue)
{
  ASSERT_TRUE(hlParser->IsFinished());
}

TEST_F(MADDY_HEADLINEPARSER, ItReplacesMarkdownWithAnHtmlHeadline)
{
  std::vector<std::string> markdown = {
    "# a", "## a", "### a", "#### a", "##### a", "###### a"
  };

  std::vector<std::string> expected = {
    "<h1>a</h1>",
    "<h2>a</h2>",
    "<h3>a</h3>",
    "<h4>a</h4>",
    "<h5>a</h5>",
    "<h6>a</h6>"
  };

  for (uint8_t i = 0; i < 6; ++i)
  {
    hlParser->AddLine(markdown[i]);
    std::stringstream& output(hlParser->GetResult());

    const std::string& outputString = output.str();

    ASSERT_EQ(expected[i], outputString);
    hlParser->Clear();
  }
}

TEST_F(MADDY_HEADLINEPARSER, ItReplacesInvalidMarkdownNotWithAnHtmlHeadline)
{
  std::string markdown = "####### a";
  std::string expected(markdown);

  hlParser->AddLine(markdown);
  std::stringstream& output(hlParser->GetResult());

  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

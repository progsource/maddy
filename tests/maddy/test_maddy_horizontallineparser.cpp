/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/horizontallineparser.h"

// -----------------------------------------------------------------------------

class MADDY_HORIZONTALLINEPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::HorizontalLineParser> hlParser;

  void SetUp() override
  {
    this->hlParser =
      std::make_shared<maddy::HorizontalLineParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_HORIZONTALLINEPARSER, IsStartingLineReturnsTrueWhenFacedWithThreeDashes
)
{
  ASSERT_TRUE(maddy::HorizontalLineParser::IsStartingLine("---"));
}

TEST_F(MADDY_HORIZONTALLINEPARSER, IsFinishedAlwaysReturnsTrue)
{
  ASSERT_TRUE(hlParser->IsFinished());
}

TEST_F(MADDY_HORIZONTALLINEPARSER, ItReplacesMarkdownWithAnHtmlLine)
{
  std::string markdown = "---";
  std::string expected = "<hr/>";

  hlParser->AddLine(markdown);
  std::stringstream& output(hlParser->GetResult());

  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_HORIZONTALLINEPARSER, ItReplacesInvalidMarkdownNotWithAnHtmlLine)
{
  std::string markdown = "--- ";
  std::string expected(markdown);

  hlParser->AddLine(markdown);
  std::stringstream& output(hlParser->GetResult());

  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/paragraphparser.h"

// -----------------------------------------------------------------------------

class MADDY_PARAGRAPHPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::ParagraphParser> pParser;

  void SetUp() override
  {
    this->pParser =
      std::make_shared<maddy::ParagraphParser>(nullptr, nullptr, true);
  }
};

// -----------------------------------------------------------------------------

TEST_F(MADDY_PARAGRAPHPARSER, IsStartingLineReturnsTrueWhenFacedWithThreeDashes)
{
  ASSERT_TRUE(maddy::ParagraphParser::IsStartingLine("---"));
}

TEST_F(MADDY_PARAGRAPHPARSER, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(pParser->IsFinished());
}

TEST_F(MADDY_PARAGRAPHPARSER, ItReplacesMarkdownWithAnHtmlLine)
{
  std::vector<std::string> markdown = {"Some text", "and some other text", ""};
  std::string expected = "<p>Some text and some other text </p>";

  for (std::string md : markdown)
  {
    pParser->AddLine(md);
  }
  ASSERT_TRUE(pParser->IsFinished());

  std::stringstream& output(pParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

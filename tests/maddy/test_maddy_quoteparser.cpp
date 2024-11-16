/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/quoteparser.h"

// -----------------------------------------------------------------------------

class MADDY_QUOTEPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::QuoteParser> quoteParser;

  void SetUp() override
  {
    this->quoteParser = std::make_shared<maddy::QuoteParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_QUOTEPARSER, IsStartingLineReturnsTrueWhenFacedWithBeginningOfAQuote
)
{
  ASSERT_TRUE(maddy::QuoteParser::IsStartingLine("> a"));
}

TEST_F(MADDY_QUOTEPARSER, IsFinishedAlwaysReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(quoteParser->IsFinished());
}

TEST_F(MADDY_QUOTEPARSER, ItReplacesMarkdownWithAnHtmlBlockQuote)
{
  std::vector<std::string> markdown = {"> a", "> b", ">", "> c", ""};
  std::string expected = "<blockquote>a b c </blockquote>";

  for (std::string md : markdown)
  {
    quoteParser->AddLine(md);
  }

  ASSERT_TRUE(quoteParser->IsFinished());

  std::stringstream& output(quoteParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

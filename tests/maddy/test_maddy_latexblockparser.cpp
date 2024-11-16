/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/latexblockparser.h"

// -----------------------------------------------------------------------------

class MADDY_LATEXBLOCKPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::LatexBlockParser> lbParser;

  void SetUp() override
  {
    this->lbParser =
      std::make_shared<maddy::LatexBlockParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(MADDY_LATEXBLOCKPARSER, IsStartingLineReturnsTrueWhenFacedWithTwoDollars)
{
  ASSERT_TRUE(maddy::LatexBlockParser::IsStartingLine("$$"));
}

TEST_F(MADDY_LATEXBLOCKPARSER, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(lbParser->IsFinished());
}

TEST_F(MADDY_LATEXBLOCKPARSER, ItReplacesOneLineMarkdownWithALatexBlock)
{
  std::vector<std::string> markdown = {
    "$$x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.$$"
  };

  std::string expected = "$$x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.$$\n";

  for (std::string md : markdown)
  {
    lbParser->AddLine(md);
  }
  ASSERT_TRUE(lbParser->IsFinished());

  std::stringstream& output(lbParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_LATEXBLOCKPARSER, ItReplacesABlockMarkdownWithALatexBlock)
{
  std::vector<std::string> markdown = {
    "$$", "x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.", "$$"
  };

  std::string expected = "$$\nx = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}.\n$$\n";

  for (std::string md : markdown)
  {
    lbParser->AddLine(md);
  }
  ASSERT_TRUE(lbParser->IsFinished());

  std::stringstream& output(lbParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_LATEXBLOCKPARSER, ItReplacesAMultilineBlockMarkdownWithALatexBlock)
{
  std::vector<std::string> markdown = {
    "$$", "x = {-b \\pm \\sqrt{b^2-4ac}", "\\over 2a}.$$"
  };

  std::string expected = "$$\nx = {-b \\pm \\sqrt{b^2-4ac}\n\\over 2a}.$$\n";

  for (std::string md : markdown)
  {
    lbParser->AddLine(md);
  }
  ASSERT_TRUE(lbParser->IsFinished());

  std::stringstream& output(lbParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

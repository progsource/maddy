/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/codeblockparser.h"

// -----------------------------------------------------------------------------

class MADDY_CODEBLOCKPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::CodeBlockParser> cbParser;

  void SetUp() override
  {
    this->cbParser = std::make_shared<maddy::CodeBlockParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(MADDY_CODEBLOCKPARSER, IsStartingLineReturnsTrueWhenFacedWithThreeSigns)
{
  ASSERT_TRUE(maddy::CodeBlockParser::IsStartingLine("```"));
}

TEST_F(MADDY_CODEBLOCKPARSER, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(cbParser->IsFinished());
}

TEST_F(MADDY_CODEBLOCKPARSER, ItReplacesMarkdownWithAnHtmlCodeBlock)
{
  std::vector<std::string> markdown = {
    "```", "some code", "some other code", "```"
  };

  std::string expected =
    "<pre><code>\nsome code\nsome other code\n</code></pre>";

  for (std::string md : markdown)
  {
    cbParser->AddLine(md);
  }
  ASSERT_TRUE(cbParser->IsFinished());

  std::stringstream& output(cbParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_CODEBLOCKPARSER, ItShouldUseAnythingBehindFirstBackticksAsClass)
{
  std::vector<std::string> markdown = {
    "```cpp", "some code", "some other code", "```"
  };

  std::string expected =
    "<pre class=\"cpp\"><code>\nsome code\nsome other code\n</code></pre>";

  for (std::string md : markdown)
  {
    cbParser->AddLine(md);
  }
  ASSERT_TRUE(cbParser->IsFinished());

  std::stringstream& output(cbParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/unorderedlistparser.h"

// -----------------------------------------------------------------------------

class MADDY_UNORDEREDLISTPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::UnorderedListParser> ulParser;

  void SetUp() override
  {
    std::function<std::shared_ptr<maddy::BlockParser>(const std::string& line)>
      getBlockParserForLineCallback = [](const std::string& line)
    {
      if (maddy::UnorderedListParser::IsStartingLine(line))
      {
        return std::static_pointer_cast<maddy::BlockParser>(
          std::make_shared<maddy::UnorderedListParser>(nullptr, nullptr)
        );
      }

      std::shared_ptr<maddy::BlockParser> empty;
      return empty;
    };

    this->ulParser = std::make_shared<maddy::UnorderedListParser>(
      nullptr, getBlockParserForLineCallback
    );
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_UNORDEREDLISTPARSER,
  IsStartingLineReturnsTrueWhenFacedWithBeginningOfList
)
{
  ASSERT_TRUE(maddy::UnorderedListParser::IsStartingLine("* a"));
}

TEST_F(MADDY_UNORDEREDLISTPARSER, IsFinishedAlwaysReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(ulParser->IsFinished());
}

TEST_F(MADDY_UNORDEREDLISTPARSER, ItReplacesMarkdownWithAnHtmlUnorderedList)
{
  std::vector<std::string> markdown = {
    "* a", "* b", "- c", "- d", "+ e", "+ f", "* g", ""
  };
  std::string expected =
    "<ul><li>a</li><li>b</li><li>c</li><li>d</li><li>e</li><li>f</li><li>g</"
    "li></ul>";

  for (std::string md : markdown)
  {
    ulParser->AddLine(md);
  }

  ASSERT_TRUE(ulParser->IsFinished());

  std::stringstream& output(ulParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_UNORDEREDLISTPARSER, ItReplacesMarkdownWithAnHierachicalHtmlList)
{
  std::vector<std::string> markdown = {
    "* a", "  * d", "  * e", "* b", "  * c", "  + x", "  + y", "  - z", ""
  };
  std::string expected =
    "<ul><li>a<ul><li>d</li><li>e</li></ul></li><li>b<ul><li>c</li><li>x</"
    "li><li>y</li><li>z</li></ul></li></ul>";

  for (std::string md : markdown)
  {
    ulParser->AddLine(md);
  }

  ASSERT_TRUE(ulParser->IsFinished());

  std::stringstream& output(ulParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

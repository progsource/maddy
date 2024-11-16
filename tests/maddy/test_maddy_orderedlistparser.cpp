/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/orderedlistparser.h"

// -----------------------------------------------------------------------------

class MADDY_ORDEREDLISTPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::OrderedListParser> olParser;

  void SetUp() override
  {
    std::function<std::shared_ptr<maddy::BlockParser>(const std::string& line)>
      getBlockParserForLineCallback = [](const std::string& line)
    {
      if (maddy::OrderedListParser::IsStartingLine(line))
      {
        return std::static_pointer_cast<maddy::BlockParser>(
          std::make_shared<maddy::OrderedListParser>(nullptr, nullptr)
        );
      }

      std::shared_ptr<maddy::BlockParser> empty;
      return empty;
    };

    this->olParser = std::make_shared<maddy::OrderedListParser>(
      nullptr, getBlockParserForLineCallback
    );
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_ORDEREDLISTPARSER, IsStartingLineReturnsTrueWhenFacedWithBeginningOfList
)
{
  ASSERT_TRUE(maddy::OrderedListParser::IsStartingLine("1. a"));
}

TEST_F(MADDY_ORDEREDLISTPARSER, IsFinishedAlwaysReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(olParser->IsFinished());
}

TEST_F(MADDY_ORDEREDLISTPARSER, ItReplacesMarkdownWithAnHtmlOrderedList)
{
  std::vector<std::string> markdown = {"1. a", "* b", ""};
  std::string expected = "<ol><li>a</li><li>b</li></ol>";

  for (std::string md : markdown)
  {
    olParser->AddLine(md);
  }

  ASSERT_TRUE(olParser->IsFinished());

  std::stringstream& output(olParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_ORDEREDLISTPARSER, ItReplacesMarkdownWithAnHierachicalHtmlList)
{
  std::vector<std::string> markdown = {
    "1. a", "  1. d", "  * e", "* b", "  1. c", ""
  };
  std::string expected =
    "<ol><li>a<ol><li>d</li><li>e</li></ol></li><li>b<ol><li>c</li></ol></li></"
    "ol>";

  for (std::string md : markdown)
  {
    olParser->AddLine(md);
  }

  ASSERT_TRUE(olParser->IsFinished());

  std::stringstream& output(olParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(
  MADDY_ORDEREDLISTPARSER, ItReplacesNumberedMarkdownListWithAnHtmlOrderedList
)
{
  std::vector<std::string> markdown = {"1. a", "94. b", "103. c", ""};
  std::string expected = "<ol><li>a</li><li>b</li><li>c</li></ol>";

  for (std::string md : markdown)
  {
    olParser->AddLine(md);
  }

  ASSERT_TRUE(olParser->IsFinished());

  std::stringstream& output(olParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

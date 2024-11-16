/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/checklistparser.h"

// -----------------------------------------------------------------------------

class MADDY_CHECKLISTPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::ChecklistParser> clParser;

  void SetUp() override
  {
    std::function<std::shared_ptr<maddy::BlockParser>(const std::string& line)>
      getBlockParserForLineCallback = [](const std::string& line)
    {
      if (maddy::ChecklistParser::IsStartingLine(line))
      {
        return std::static_pointer_cast<maddy::BlockParser>(
          std::make_shared<maddy::ChecklistParser>(nullptr, nullptr)
        );
      }

      std::shared_ptr<maddy::BlockParser> empty;
      return empty;
    };

    this->clParser = std::make_shared<maddy::ChecklistParser>(
      nullptr, getBlockParserForLineCallback
    );
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_CHECKLISTPARSER, IsStartingLineReturnsTrueWhenFacedWithBeginningOfList
)
{
  ASSERT_TRUE(maddy::ChecklistParser::IsStartingLine("- [ ] a"));
  ASSERT_TRUE(maddy::ChecklistParser::IsStartingLine("- [x] b"));
}

TEST_F(MADDY_CHECKLISTPARSER, IsFinishedAlwaysReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(clParser->IsFinished());
}

TEST_F(MADDY_CHECKLISTPARSER, ItReplacesMarkdownWithAnHtmlChecklist)
{
  std::vector<std::string> markdown = {"- [ ] a", "- [x] b", ""};
  std::string expected =
    "<ul class=\"checklist\"><li><label><input type=\"checkbox\"/> "
    "a</label></li><li><label><input type=\"checkbox\" checked=\"checked\"/> "
    "b</label></li></ul>";

  for (std::string md : markdown)
  {
    clParser->AddLine(md);
  }

  ASSERT_TRUE(clParser->IsFinished());

  std::stringstream& output(clParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

TEST_F(MADDY_CHECKLISTPARSER, ItReplacesMarkdownWithAnHierachicalHtmlList)
{
  std::vector<std::string> markdown = {
    "- [ ] a", "  - [ ] d", "  - [ ] e", "- [ ] b", "  - [x] c", ""
  };
  std::string expected =
    "<ul class=\"checklist\"><li><label><input type=\"checkbox\"/> a<ul "
    "class=\"checklist\"><li><label><input type=\"checkbox\"/> "
    "d</label></li><li><label><input type=\"checkbox\"/> "
    "e</label></li></ul></label></li><li><label><input type=\"checkbox\"/> "
    "b<ul class=\"checklist\"><li><label><input type=\"checkbox\" "
    "checked=\"checked\"/> c</label></li></ul></label></li></ul>";

  for (std::string md : markdown)
  {
    clParser->AddLine(md);
  }

  ASSERT_TRUE(clParser->IsFinished());

  std::stringstream& output(clParser->GetResult());
  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

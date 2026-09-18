/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/tableparser.h"

// -----------------------------------------------------------------------------

class MADDY_TABLEPARSER : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::TableParser> tableParser;

  void SetUp() override
  {
    this->tableParser = std::make_shared<maddy::TableParser>(nullptr, nullptr);
  }
};

// -----------------------------------------------------------------------------

TEST_F(
  MADDY_TABLEPARSER, IsStartingLineReturnsTrueWhenFacedWithTheBeginningOfATable
)
{
  ASSERT_EQ(true, maddy::TableParser::IsStartingLine("|table>"));
}

TEST_F(MADDY_TABLEPARSER, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(tableParser->IsFinished());
}

TEST_F(MADDY_TABLEPARSER, ItReplacesMarkdownWithAnHtmlTable)
{
  std::vector<std::string> markdown = {
    "|table>",
    "Left header|middle header|last header",
    "- | - | -",
    "cell 1|cell 2|cell 3",
    "cell 4|cell 5|cell 6",
    "- | - | -",
    "foot a|foot b|foot c",
    "|<table"
  };
  std::string expected =
    "<table><thead><tr><th>Left header</th><th>middle header</th><th>last "
    "header</th></tr></thead><tbody><tr><td>cell 1</td><td>cell 2</td><td>cell "
    "3</td></tr><tr><td>cell 4</td><td>cell 5</td><td>cell "
    "6</td></tr></tbody><tfoot><tr><td>foot a</td><td>foot b</td><td>foot "
    "c</td></tr></tfoot></table>";

  for (std::string md : markdown)
  {
    tableParser->AddLine(md);
  }

  ASSERT_TRUE(tableParser->IsFinished());

  std::stringstream& output(tableParser->GetResult());

  const std::string& outputString = output.str();

  ASSERT_EQ(expected, outputString);
}

// -----------------------------------------------------------------------------
// GFM pipe-table mode (useMaddySpecificMarkdown = false)
// -----------------------------------------------------------------------------

class MADDY_TABLEPARSER_GFM : public ::testing::Test
{
protected:
  std::shared_ptr<maddy::TableParser> tableParser;

  void SetUp() override
  {
    this->tableParser =
      std::make_shared<maddy::TableParser>(nullptr, nullptr, false);
  }
};

TEST_F(MADDY_TABLEPARSER_GFM, IsStartingLineReturnsTrueForAnyLineWithAPipe)
{
  ASSERT_TRUE(maddy::TableParser::IsStartingLine("| a | b |", false));
}

TEST_F(MADDY_TABLEPARSER_GFM, IsStartingLineReturnsFalseForABlankLine)
{
  ASSERT_FALSE(maddy::TableParser::IsStartingLine("   ", false));
}

TEST_F(MADDY_TABLEPARSER_GFM, IsFinishedReturnsFalseInTheBeginning)
{
  ASSERT_FALSE(tableParser->IsFinished());
}

TEST_F(MADDY_TABLEPARSER_GFM, ItReplacesMarkdownWithAnHtmlTableAndHasNoFooter)
{
  std::vector<std::string> markdown = {
    "| Left header | middle header | last header |",
    "| --- | --- | --- |",
    "| cell 1 | cell 2 | cell 3 |",
    "| cell 4 | cell 5 | cell 6 |",
    ""
  };
  std::string expected =
    "<table><thead><tr><th>Left header</th><th>middle header</th><th>last "
    "header</th></tr></thead><tbody><tr><td>cell 1</td><td>cell 2</td><td>cell "
    "3</td></tr><tr><td>cell 4</td><td>cell 5</td><td>cell "
    "6</td></tr></tbody></table>";

  for (std::string md : markdown)
  {
    tableParser->AddLine(md);
  }

  ASSERT_TRUE(tableParser->IsFinished());
  ASSERT_EQ(expected, tableParser->GetResult().str());
}

TEST_F(
  MADDY_TABLEPARSER_GFM,
  ItFallsBackToAParagraphWhenTheSecondLineIsNotASeparatorRow
)
{
  std::string first = "not | a | table";
  std::string second = "just some more text";
  std::string third = "";
  std::string expected = "<p>not | a | table just some more text </p>";

  tableParser->AddLine(first);
  tableParser->AddLine(second);

  if (!tableParser->IsFinished())
  {
    tableParser->AddLine(third);
  }

  ASSERT_TRUE(tableParser->IsFinished());
  ASSERT_EQ(expected, tableParser->GetResult().str());
}

TEST_F(MADDY_TABLEPARSER_GFM, OldMaddySpecificSyntaxIsNotRecognizedAsATable)
{
  std::string first = "|table>";
  std::string second = "Left header|middle header|last header";
  std::string third = "";

  tableParser->AddLine(first);
  tableParser->AddLine(second);

  if (!tableParser->IsFinished())
  {
    tableParser->AddLine(third);
  }

  ASSERT_TRUE(tableParser->IsFinished());
  ASSERT_EQ(std::string::npos, tableParser->GetResult().str().find("<table>"));
}

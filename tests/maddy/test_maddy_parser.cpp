/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */

#include "maddy/test_maddy_parser.h"

#include "gmock/gmock.h"

#include "maddy/parser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_PARSER, ItShouldParse)
{
  auto parser = std::make_shared<maddy::Parser>();
  std::stringstream markdown(testMarkdown);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(testHtml, output);
}

TEST(MADDY_PARSER, ItShouldParseWithBitwiseConfig)
{
  auto config = std::make_shared<maddy::ParserConfig>();
  config->enabledParsers &= ~maddy::types::EMPHASIZED_PARSER;
  config->enabledParsers |= maddy::types::HTML_PARSER;

  auto parser = std::make_shared<maddy::Parser>(config);

  std::stringstream markdown(testMarkdown);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(testHtml2, output);
}

TEST(MADDY_PARSER, ItShouldParseWithSmallConfig)
{
  auto config = std::make_shared<maddy::ParserConfig>();
  config->enabledParsers =
    maddy::types::EMPHASIZED_PARSER | maddy::types::STRONG_PARSER;

  auto parser = std::make_shared<maddy::Parser>(config);

  std::stringstream markdown(testMarkdown);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(testHtml3, output);
}

TEST(MADDY_PARSER, ItShouldParseInlineCodeInHeadlines)
{
  const std::string headlineTest = R"(
# Some **test** markdown
)";
  const std::string expectedHTML =
    "<h1>Some <strong>test</strong> markdown</h1>";
  std::stringstream markdown(headlineTest);

  auto parser = std::make_shared<maddy::Parser>();

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(expectedHTML, output);
}

TEST(MADDY_PARSER, ItShouldNotParseInlineCodeInHeadlineIfDisabled)
{
  const std::string headlineTest = R"(
# Some **test** markdown
)";
  const std::string expectedHTML = "<h1>Some **test** markdown</h1>";
  std::stringstream markdown(headlineTest);
  auto config = std::make_shared<maddy::ParserConfig>();
  config->isHeadlineInlineParsingEnabled = false;
  auto parser = std::make_shared<maddy::Parser>(config);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(expectedHTML, output);
}

TEST(MADDY_PARSER, ItShouldParseGfmTablesWhenMaddySpecificParserIsDisabled)
{
  const std::string tableTest =
    "| Left header | middle header | last header |\n"
    "| --- | --- | --- |\n"
    "| cell 1 | cell 2 | cell 3 |\n"
    "| cell 4 | cell 5 | cell 6 |\n";
  const std::string expectedHTML =
    "<table><thead><tr><th>Left header</th><th>middle header</th><th>last "
    "header</th></tr></thead><tbody><tr><td>cell 1</td><td>cell 2</td><td>cell "
    "3</td></tr><tr><td>cell 4</td><td>cell 5</td><td>cell "
    "6</td></tr></tbody></table>";
  std::stringstream markdown(tableTest);
  auto config = std::make_shared<maddy::ParserConfig>();
  config->enabledParsers &= ~maddy::types::MADDY_SPECIFIC_PARSER;
  auto parser = std::make_shared<maddy::Parser>(config);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(expectedHTML, output);
}

TEST(
  MADDY_PARSER,
  ItShouldNotParseMaddySpecificTableSyntaxWhenMaddySpecificParserIsDisabled
)
{
  const std::string tableTest =
    "|table>\n"
    "A|B\n"
    "- | - | -\n"
    "1|2\n"
    "|<table\n";
  std::stringstream markdown(tableTest);
  auto config = std::make_shared<maddy::ParserConfig>();
  config->enabledParsers &= ~maddy::types::MADDY_SPECIFIC_PARSER;
  auto parser = std::make_shared<maddy::Parser>(config);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(std::string::npos, output.find("<table>"));
}

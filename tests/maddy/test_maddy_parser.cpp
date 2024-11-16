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

TEST(MADDY_PARSER, ItShouldParseWithConfig)
{
  auto config = std::make_shared<maddy::ParserConfig>();
  config->isEmphasizedParserEnabled = false;
  config->isHTMLWrappedInParagraph = false;

  auto parser = std::make_shared<maddy::Parser>(config);

  std::stringstream markdown(testMarkdown);

  const std::string output = parser->Parse(markdown);

  ASSERT_EQ(testHtml2, output);
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

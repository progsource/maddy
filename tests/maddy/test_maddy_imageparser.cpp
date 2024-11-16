/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#include <memory>

#include "gmock/gmock.h"

#include "maddy/imageparser.h"

// -----------------------------------------------------------------------------

TEST(MADDY_IMAGEPARSER, ItReplacesMarkdownWithAnImage)
{
  std::string text = "Some text ![Image Title](http://example.com/a.png)";
  std::string expected =
    "Some text <img src=\"http://example.com/a.png\" alt=\"Image Title\"/>";
  auto imageParser = std::make_shared<maddy::ImageParser>();

  imageParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_IMAGEPARSER, ItReplacesMarkdownWithImages)
{
  std::string text =
    "Some text ![Image Title](http://example.com/a.png) bla ![Image "
    "Title](http://example.com/a.png)";
  std::string expected =
    "Some text <img src=\"http://example.com/a.png\" alt=\"Image Title\"/> bla "
    "<img src=\"http://example.com/a.png\" alt=\"Image Title\"/>";
  auto imageParser = std::make_shared<maddy::ImageParser>();

  imageParser->Parse(text);

  ASSERT_EQ(expected, text);
}

TEST(MADDY_IMAGEPARSER, ItReplacesNoLinkMarkdownWithImages)
{
  std::string text = "Some text [Image Title](http://example.com)";
  std::string expected(text);
  auto imageParser = std::make_shared<maddy::ImageParser>();

  imageParser->Parse(text);

  ASSERT_EQ(expected, text);
}

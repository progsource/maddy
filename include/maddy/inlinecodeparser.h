/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <regex>
#include <string>

#include "maddy/common.h"
#include "maddy/lineparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * InlineCodeParser
 *
 * @class
 */
class InlineCodeParser : public LineParser
{
public:
  /**
   * Parse
   *
   * From Markdown: `text `some code``
   *
   * To HTML: `text <code>some code</code>`
   *
   * @method
   * @param {std::string&} line The line to interpret
   * @return {void}
   */
  void Parse(std::string& line) override
  {
    static std::regex re("`([^`]*)`");
    std::smatch match;
    std::string result;

    auto searchStart = line.cbegin();

    while (std::regex_search(searchStart, line.cend(), match, re))
    {
      result.append(match.prefix());

      result += "<code>" + common::escapeHTML(match[1].str()) + "</code>";

      searchStart = match.suffix().first;
    }

    result.append(searchStart, line.cend());

    line = std::move(result);
  }
}; // class InlineCodeParser

// -----------------------------------------------------------------------------

} // namespace maddy

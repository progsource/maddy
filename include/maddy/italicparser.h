/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <regex>
#include <string>

#include "maddy/codespanutils.h"
#include "maddy/lineparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * ItalicParser
 *
 * @class
 */
class ItalicParser : public LineParser
{
public:
  /**
   * Parse
   *
   * From Markdown: `text *text*`
   *
   * To HTML: `text <i>text</i>`
   *
   * @method
   * @param {std::string&} line The line to interpret
   * @return {void}
   */
  void Parse(std::string& line) override
  {
    static std::regex re(R"(\*([^\*]*)\*)");
    static std::string replacement = "<i>$1</i>";

    ApplyOutsideProtectedSpans(
      line,
      [](std::string& segment)
      { segment = std::regex_replace(segment, re, replacement); }
    );
  }
}; // class ItalicParser

// -----------------------------------------------------------------------------

} // namespace maddy

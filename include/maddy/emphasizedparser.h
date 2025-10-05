/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <regex>
#include <string>

#include "maddy/lineparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * EmphasizedParser
 *
 * Has to be used after the `StrongParser`.
 *
 * @class
 */
class EmphasizedParser : public LineParser
{
public:
  /**
   * Parse
   *
   * From Markdown: `text _text_`
   *
   * To HTML: `text <em>text</em>`
   *
   * @method
   * @param {std::string&} line The line to interpret
   * @return {void}
   */
  void Parse(std::string& line) override
  {
    // Modifed from previous version, with help from
    // https://stackoverflow.com/questions/61346949/regex-for-markdown-emphasis
    static std::regex re(
      R"((?!.*`.*|.*<code>.*)\b_(?![\s])(?!.*`.*|.*<\/code>.*)(.*?[^\s])_\b(?!.*`.*|.*<\/code>.*))"
    );
    static std::string replacement = "<em>$1</em>";

    line = std::regex_replace(line, re, replacement);
  }
}; // class EmphasizedParser

// -----------------------------------------------------------------------------

} // namespace maddy

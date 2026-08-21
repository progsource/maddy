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
    // The leading and trailing `(_*)` groups absorb any leftover underscores
    // from an unbalanced run (e.g. `__foo_` or `_foo____`), re-emitted
    // outside the <em> tag instead of into its content.
    static std::regex re(
      R"((?!.*`.*|.*<code>.*)\b(_*)_(?![\s_])(?!.*`.*|.*<\/code>.*)(.*?[^\s])_(_*)\b(?!.*`.*|.*<\/code>.*))"
    );
    static std::string replacement = "$1<em>$2</em>$3";

    line = std::regex_replace(line, re, replacement);
  }
}; // class EmphasizedParser

// -----------------------------------------------------------------------------

} // namespace maddy

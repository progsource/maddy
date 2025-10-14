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
 * StrongParser
 *
 * Has to be used before the `EmphasizedParser`.
 *
 * @class
 */
class StrongParser : public LineParser
{
public:
  /**
   * Parse
   *
   * From Markdown: `text **text** __text__`
   *
   * To HTML: `text <strong>text</strong> <strong>text</strong>`
   *
   * @method
   * @param {std::string&} line The line to interpret
   * @return {void}
   */
  void Parse(std::string& line) override
  {
    // This version of the regex is changed exactly the same way
    // that the regex for the emphasized parser was changed, and
    // it then passes all the 'disabled' tests in the 'strong parser'
    // test, but then it fails general parsing.  For some reason,
    // "__text__" translates "<i></i>text<i></i>" even though there
    // are no word boundaries at the correct places.  It's weird!

    // static std::vector<std::regex> res{
    //   std::regex{
    //     R"((?!.*`.*|.*<code>.*)\b\*\*(?![\s])(?!.*`.*|.*<\/code>.*)"
    //      "(.*?[^\s])\*\*\b(?!.*`.*|.*<\/code>.*))"
    //   },
    //   std::regex{
    //     R"((?!.*`.*|.*<code>.*)\b__(?![\s])(?!.*`.*|.*<\/code>.*)"
    //      "(.*?[^\s])__\b(?!.*`.*|.*<\/code>.*))"
    //   }
    // };
    static std::vector<std::regex> res{
      std::regex{
        R"((?!.*`.*|.*<code>.*)\*\*(?!.*`.*|.*<\/code>.*)([^\*\*]*)\*\*(?!.*`.*|.*<\/code>.*))"
      },
      std::regex{
        R"((?!.*`.*|.*<code>.*)__(?!.*`.*|.*<\/code>.*)([^__]*)__(?!.*`.*|.*<\/code>.*))"
      }
    };
    static std::string replacement = "<strong>$1</strong>";
    for (const auto& re : res)
    {
      line = std::regex_replace(line, re, replacement);
    }
  }
}; // class StrongParser

// -----------------------------------------------------------------------------

} // namespace maddy

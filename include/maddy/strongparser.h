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
    // `*` is not a word character, so `\b` next to it does not mean
    // "edge of a delimiter run" the way it does for `_`; the asterisk
    // variant is left without a word-boundary anchor.
    static std::regex reAsterisk{R"(\*\*(?![\s])(.*?[^\s])\*\*)"};
    // The leading and trailing `(_*)` groups absorb any leftover underscores
    // from an unbalanced run on either side (e.g. `___text__` or
    // `__text_______`), re-emitted outside the <strong> tag by the caller
    // instead of being swallowed into its content.
    static std::regex reUnderscore{R"(\b(_*)__(?![\s_])(.*?[^\s])__(_*)\b)"};

    ApplyOutsideProtectedSpans(
      line,
      [](std::string& segment)
      {
        segment =
          std::regex_replace(segment, reAsterisk, "<strong>$1</strong>");
        segment =
          std::regex_replace(segment, reUnderscore, "$1<strong>$2</strong>$3");
      }
    );
  }
}; // class StrongParser

// -----------------------------------------------------------------------------

} // namespace maddy

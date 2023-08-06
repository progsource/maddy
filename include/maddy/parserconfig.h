/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

#include <stdint.h>

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

namespace types {

/**
 * PARSER_TYPE
 *
 * Bitwise flags to turn on/off each parser
*/
enum PARSER_TYPE : uint32_t
{
  NONE                     = 0,

  BREAKLINE_PARSER         = 0b1,
  CHECKLIST_PARSER         = 0b10,
  CODE_BLOCK_PARSER        = 0b100,
  EMPHASIZED_PARSER        = 0b1000,
  HEADLINE_PARSER          = 0b10000,
  HORIZONTAL_LINE_PARSER   = 0b100000,
  HTML_PARSER              = 0b1000000,
  IMAGE_PARSER             = 0b10000000,
  INLINE_CODE_PARSER       = 0b100000000,
  ITALIC_PARSER            = 0b1000000000,
  LINK_PARSER              = 0b10000000000,
  ORDERED_LIST_PARSER      = 0b100000000000,
  PARAGRAPH_PARSER         = 0b1000000000000,
  QUOTE_PARSER             = 0b10000000000000,
  STRIKETHROUGH_PARSER     = 0b100000000000000,
  STRONG_PARSER            = 0b1000000000000000,
  TABLE_PARSER             = 0b10000000000000000,
  UNORDERED_LIST_PARSER    = 0b100000000000000000,
  LATEX_BLOCK_PARSER       = 0b1000000000000000000,

  DEFAULT                  = 0b0111111111110111111,
  ALL                      = 0b1111111111111111111,
};

} // namespace types

/**
 * ParserConfig
 *
 * @class
 */
struct ParserConfig
{
  /**
   * @deprecated will be removed in 1.4.0 latest
   *
   * this flag = false == `enabledParsers &= ~maddy::types::EMPHASIZED_PARSER`
  */
  bool isEmphasizedParserEnabled;

  /**
   * @deprecated will be removed in 1.4.0 latest
   *
   * this flag = false == `enabledParsers |= maddy::types::HTML_PARSER`
  */
  bool isHTMLWrappedInParagraph;

  uint32_t enabledParsers;

  ParserConfig()
    : isEmphasizedParserEnabled(true)
    , isHTMLWrappedInParagraph(true)
    , enabledParsers(maddy::types::DEFAULT)
  {}
}; // class ParserConfig

// -----------------------------------------------------------------------------

} // namespace maddy

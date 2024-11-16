/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <functional>
#include <regex>
#include <string>

#include "maddy/blockparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * ChecklistParser
 *
 * @class
 */
class ChecklistParser : public BlockParser
{
public:
  /**
   * ctor
   *
   * @method
   * @param {std::function<void(std::string&)>} parseLineCallback
   * @param {std::function<std::shared_ptr<BlockParser>(const std::string&
   * line)>} getBlockParserForLineCallback
   */
  ChecklistParser(
    std::function<void(std::string&)> parseLineCallback,
    std::function<std::shared_ptr<BlockParser>(const std::string& line)>
      getBlockParserForLineCallback
  )
    : BlockParser(parseLineCallback, getBlockParserForLineCallback)
    , isStarted(false)
    , isFinished(false)
  {}

  /**
   * IsStartingLine
   *
   * An unordered list starts with `* `.
   *
   * @method
   * @param {const std::string&} line
   * @return {bool}
   */
  static bool IsStartingLine(const std::string& line)
  {
    static std::regex re(R"(^- \[[x| ]\] .*)");
    return std::regex_match(line, re);
  }

  /**
   * IsFinished
   *
   * @method
   * @return {bool}
   */
  bool IsFinished() const override { return this->isFinished; }

protected:
  bool isInlineBlockAllowed() const override { return true; }

  bool isLineParserAllowed() const override { return true; }

  void parseBlock(std::string& line) override
  {
    bool isStartOfNewListItem = IsStartingLine(line);
    uint32_t indentation = getIndentationWidth(line);

    static std::regex lineRegex("^(- )");
    line = std::regex_replace(line, lineRegex, "");

    static std::regex emptyBoxRegex(R"(^\[ \])");
    static std::string emptyBoxReplacement = "<input type=\"checkbox\"/>";
    line = std::regex_replace(line, emptyBoxRegex, emptyBoxReplacement);

    static std::regex boxRegex(R"(^\[x\])");
    static std::string boxReplacement =
      "<input type=\"checkbox\" checked=\"checked\"/>";
    line = std::regex_replace(line, boxRegex, boxReplacement);

    if (!this->isStarted)
    {
      line = "<ul class=\"checklist\"><li><label>" + line;
      this->isStarted = true;
      return;
    }

    if (indentation >= 2)
    {
      line = line.substr(2);
      return;
    }

    if (line.empty() ||
        line.find("</label></li><li><label>") != std::string::npos ||
        line.find("</label></li></ul>") != std::string::npos)
    {
      line = "</label></li></ul>" + line;
      this->isFinished = true;
      return;
    }

    if (isStartOfNewListItem)
    {
      line = "</label></li><li><label>" + line;
    }
  }

private:
  bool isStarted;
  bool isFinished;
}; // class ChecklistParser

// -----------------------------------------------------------------------------

} // namespace maddy

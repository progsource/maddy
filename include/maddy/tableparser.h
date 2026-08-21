/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <functional>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "maddy/blockparser.h"
#include "maddy/paragraphparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * TableParser
 *
 * Supports two independent syntaxes, chosen with `useMaddySpecificMarkdown`
 * (see `maddy::types::MADDY_SPECIFIC_PARSER`).
 *
 * When true (the default, and maddy's original behavior), a table uses
 * maddy's own sigils:
 *
 * ```
 * |table>
 * Left header|middle header|last header
 * - | - | -
 * Cell A1|Cell B1|Cell C1
 * - | - | -
 * Foot A|Foot B|Foot C
 * |<table
 * ```
 *
 * A second `- | - | -` row before `|<table` marks an optional footer.
 *
 * When false, a table is a GitHub-flavored-Markdown pipe table instead:
 *
 * ```
 * | Header A | Header B |
 * | -------- | -------- |
 * | Cell A1  | Cell B1  |
 * ```
 *
 * GFM tables have no footer concept, so this mode never produces a
 * `<tfoot>`. Since `IsStartingLine` only sees one line at a time in this
 * mode, it can't yet tell a table header from an ordinary line that happens
 * to contain a `|`; if the following line isn't a valid separator row, both
 * lines are handed off to a ParagraphParser instead.
 *
 * @class
 */
class TableParser : public BlockParser
{
public:
  /**
   * ctor
   *
   * @method
   * @param {std::function<void(std::string&)>} parseLineCallback
   * @param {std::function<std::shared_ptr<BlockParser>(const std::string&
   * line)>} getBlockParserForLineCallback
   * @param {bool} useMaddySpecificMarkdown
   */
  TableParser(
    std::function<void(std::string&)> parseLineCallback,
    std::function<std::shared_ptr<BlockParser>(const std::string& line)>
      getBlockParserForLineCallback,
    bool useMaddySpecificMarkdown = true
  )
    : BlockParser(parseLineCallback, getBlockParserForLineCallback)
    , useMaddySpecificMarkdown(useMaddySpecificMarkdown)
    , isStarted(false)
    , isFinished(false)
    , currentBlock(0)
    , currentRow(0)
    , gfmState(GfmState::EXPECT_HEADER)
  {}

  /**
   * IsStartingLine
   *
   * With maddy-specific markdown, a table starts with exact `|table>`.
   * With GFM markdown, a table can only start with a row that has at least
   * one `|` separating two cells; whether it really is a table is only
   * known once the following line (the separator row) has been seen.
   *
   * @method
   * @param {const std::string&} line
   * @param {bool} useMaddySpecificMarkdown
   * @return {bool}
   */
  static bool IsStartingLine(
    const std::string& line, bool useMaddySpecificMarkdown = true
  )
  {
    if (useMaddySpecificMarkdown)
    {
      static std::string matchString("|table>");
      return line == matchString;
    }

    return IsTableRow(line);
  }

  /**
   * AddLine
   *
   * Adding a line which has to be parsed.
   *
   * @method
   * @param {std::string&} line
   * @return {void}
   */
  void AddLine(std::string& line) override
  {
    if (this->useMaddySpecificMarkdown)
    {
      this->AddLineMaddyStyle(line);
    }
    else
    {
      this->AddLineGfm(line);
    }
  }

  /**
   * IsFinished
   *
   * With maddy-specific markdown, a table ends with `|<table`. With GFM
   * markdown, a table ends with a blank line, or at the end of input.
   *
   * @method
   * @return {bool}
   */
  bool IsFinished() const override { return this->isFinished; }

protected:
  bool isInlineBlockAllowed() const override { return false; }

  bool isLineParserAllowed() const override { return false; }

  // Only used by the maddy-specific-markdown mode; the GFM mode writes
  // directly into `result` from `AddLineGfm` instead.
  void parseBlock(std::string&) override
  {
    result << "<table>";

    bool hasHeader = false;
    bool hasFooter = false;
    bool isFirstBlock = true;
    uint32_t currentBlockNumber = 0;

    if (this->table.size() > 1)
    {
      hasHeader = true;
    }

    if (this->table.size() >= 3)
    {
      hasFooter = true;
    }

    for (const std::vector<std::vector<std::string>>& block : this->table)
    {
      bool isInHeader = false;
      bool isInFooter = false;
      ++currentBlockNumber;

      if (hasHeader && isFirstBlock)
      {
        result << "<thead>";
        isInHeader = true;
      }
      else if (hasFooter && currentBlockNumber == this->table.size())
      {
        result << "<tfoot>";
        isInFooter = true;
      }
      else
      {
        result << "<tbody>";
      }

      for (const std::vector<std::string>& row : block)
      {
        result << "<tr>";

        for (const std::string& column : row)
        {
          if (isInHeader)
          {
            result << "<th>";
          }
          else
          {
            result << "<td>";
          }

          result << column;

          if (isInHeader)
          {
            result << "</th>";
          }
          else
          {
            result << "</td>";
          }
        }

        result << "</tr>";
      }

      if (isInHeader)
      {
        result << "</thead>";
      }
      else if (isInFooter)
      {
        result << "</tfoot>";
      }
      else
      {
        result << "</tbody>";
      }

      isFirstBlock = false;
    }

    result << "</table>";
  }

private:
  bool useMaddySpecificMarkdown;

  // --- maddy-specific-markdown mode state ---
  bool isStarted;
  bool isFinished;
  uint32_t currentBlock;
  uint32_t currentRow;
  std::vector<std::vector<std::vector<std::string>>> table;

  void AddLineMaddyStyle(std::string& line)
  {
    if (!this->isStarted && line == "|table>")
    {
      this->isStarted = true;
      return;
    }

    if (this->isStarted)
    {
      if (line == "- | - | -")
      {
        ++this->currentBlock;
        this->currentRow = 0;
        return;
      }

      if (line == "|<table")
      {
        static std::string emptyLine = "";
        this->parseBlock(emptyLine);
        this->isFinished = true;
        return;
      }

      if (this->table.size() < this->currentBlock + 1)
      {
        this->table.push_back(std::vector<std::vector<std::string>>());
      }
      this->table[this->currentBlock].push_back(std::vector<std::string>());

      std::string segment;
      std::stringstream streamToSplit(line);

      while (std::getline(streamToSplit, segment, '|'))
      {
        this->parseLine(segment);
        this->table[this->currentBlock][this->currentRow].push_back(segment);
      }

      ++this->currentRow;
    }
  }

  // --- GFM-pipe-table mode state ---
  enum class GfmState
  {
    EXPECT_HEADER,
    EXPECT_SEPARATOR,
    IN_BODY
  };

  GfmState gfmState;
  std::string headerLine;
  std::shared_ptr<BlockParser> fallbackParser;

  void AddLineGfm(std::string& line)
  {
    if (this->fallbackParser)
    {
      this->fallbackParser->AddLine(line);

      if (this->fallbackParser->IsFinished())
      {
        this->result << this->fallbackParser->GetResult().str();
        this->isFinished = true;
      }

      return;
    }

    switch (this->gfmState)
    {
      case GfmState::EXPECT_HEADER:
        this->headerLine = line;
        this->gfmState = GfmState::EXPECT_SEPARATOR;
        return;

      case GfmState::EXPECT_SEPARATOR:
        if (IsSeparatorRow(line) &&
            SplitRow(line).size() == SplitRow(this->headerLine).size())
        {
          this->WriteGfmHeader();
          this->gfmState = GfmState::IN_BODY;
        }
        else
        {
          this->FallBackToParagraph(line);
        }
        return;

      case GfmState::IN_BODY:
        if (line.empty())
        {
          this->result << "</tbody></table>";
          this->isFinished = true;
        }
        else
        {
          this->WriteGfmRow(line);
        }
        return;
    }
  }

  static bool IsTableRow(const std::string& line)
  {
    return line.find('|') != std::string::npos &&
           line.find_first_not_of(" \t") != std::string::npos;
  }

  static bool IsSeparatorRow(const std::string& line)
  {
    if (!IsTableRow(line))
    {
      return false;
    }

    static const std::regex cellRe("^:?-+:?$");

    for (const std::string& cell : SplitRow(line))
    {
      if (!std::regex_match(cell, cellRe))
      {
        return false;
      }
    }

    return true;
  }

  static std::vector<std::string> SplitRow(const std::string& line)
  {
    std::vector<std::string> cells;
    std::stringstream stream(line);
    std::string cell;

    while (std::getline(stream, cell, '|'))
    {
      Trim(cell);

      if (!cell.empty())
      {
        cells.push_back(cell);
      }
    }

    return cells;
  }

  static void Trim(std::string& str)
  {
    size_t first = str.find_first_not_of(" \t");

    if (first == std::string::npos)
    {
      str.clear();
      return;
    }

    size_t last = str.find_last_not_of(" \t");
    str = str.substr(first, last - first + 1);
  }

  void WriteGfmHeader()
  {
    this->result << "<table><thead><tr>";

    for (std::string cell : SplitRow(this->headerLine))
    {
      this->parseLine(cell);
      this->result << "<th>" << cell << "</th>";
    }

    this->result << "</tr></thead><tbody>";
  }

  void WriteGfmRow(const std::string& line)
  {
    this->result << "<tr>";

    for (std::string cell : SplitRow(line))
    {
      this->parseLine(cell);
      this->result << "<td>" << cell << "</td>";
    }

    this->result << "</tr>";
  }

  void FallBackToParagraph(const std::string& secondLine)
  {
    this->fallbackParser = std::make_shared<ParagraphParser>(
      [this](std::string& l) { this->parseLine(l); }, nullptr, true
    );

    std::string first = this->headerLine;
    this->fallbackParser->AddLine(first);

    std::string second = secondLine;
    this->fallbackParser->AddLine(second);

    if (this->fallbackParser->IsFinished())
    {
      this->result << this->fallbackParser->GetResult().str();
      this->isFinished = true;
    }
  }
}; // class TableParser

// -----------------------------------------------------------------------------

} // namespace maddy

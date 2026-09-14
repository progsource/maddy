/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <functional>
#include <regex>
#include <string>
#include <vector>

#include "maddy/blockparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * GfmTableParser
 *
 * Parses GitHub Flavored Markdown (GFM) tables.
 *
 * A GFM table starts with a line like `| Header | Header |`,
 * followed by a separator line like `| --- | --- |`,
 * then data rows like `| Cell | Cell |`.
 *
 * @class
 */
class GfmTableParser : public BlockParser
{
public:
  GfmTableParser(
    std::function<void(std::string&)> parseLineCallback,
    std::function<std::shared_ptr<BlockParser>(const std::string& line)>
      getBlockParserForLineCallback
  )
    : BlockParser(parseLineCallback, getBlockParserForLineCallback)
    , isFinished_(false)
    , lineIndex_(0)
  {}

  /**
   * IsStartingLine
   *
   * A GFM table starts with a line that begins and ends with `|` and
   * contains at least one inner `|` separator.
   */
  static bool IsStartingLine(const std::string& line)
  {
    // Must start with optional whitespace then `|`
    auto trimmed = trimWhitespace(line);
    if (trimmed.size() < 5) return false;  // minimum: `| a |`
    if (trimmed.front() != '|') return false;
    if (trimmed.back() != '|') return false;
    // Must have at least one inner `|` (i.e. at least 2 cells)
    // or just one cell like `| foo |` is also valid
    return true;
  }

  void AddLine(std::string& line) override
  {
    auto trimmed = trimWhitespace(line);

    // If we already started and the line doesn't look like a table row, finish.
    if (lineIndex_ > 0 && (trimmed.empty() || trimmed.front() != '|'))
    {
      finish();
      return;
    }

    if (lineIndex_ == 0)
    {
      // Header row
      headerCells_ = splitRow(trimmed);
      ++lineIndex_;
      return;
    }

    if (lineIndex_ == 1)
    {
      // Separator row — parse alignment
      auto separators = splitRow(trimmed);
      for (const auto& sep : separators)
      {
        auto s = trimWhitespace(sep);
        bool leftColon = !s.empty() && s.front() == ':';
        bool rightColon = !s.empty() && s.back() == ':';

        if (leftColon && rightColon)
          alignments_.push_back("center");
        else if (rightColon)
          alignments_.push_back("right");
        else
          alignments_.push_back("left");
      }
      ++lineIndex_;
      return;
    }

    // Data row
    bodyRows_.push_back(splitRow(trimmed));
    ++lineIndex_;
  }

  bool IsFinished() const override { return this->isFinished_; }

protected:
  bool isInlineBlockAllowed() const override { return false; }
  bool isLineParserAllowed() const override { return true; }

  void parseBlock(std::string&) override
  {
    result << "<table>";

    // thead
    result << "<thead><tr>";
    for (size_t i = 0; i < headerCells_.size(); ++i)
    {
      std::string cell = headerCells_[i];
      this->parseLine(cell);
      result << "<th";
      if (i < alignments_.size() && alignments_[i] != "left")
      {
        result << " align=\"" << alignments_[i] << "\"";
      }
      result << ">" << cell << "</th>";
    }
    result << "</tr></thead>";

    // tbody
    if (!bodyRows_.empty())
    {
      result << "<tbody>";
      for (const auto& row : bodyRows_)
      {
        result << "<tr>";
        for (size_t i = 0; i < row.size(); ++i)
        {
          std::string cell = row[i];
          this->parseLine(cell);
          result << "<td";
          if (i < alignments_.size() && alignments_[i] != "left")
          {
            result << " align=\"" << alignments_[i] << "\"";
          }
          result << ">" << cell << "</td>";
        }
        // Pad missing cells
        for (size_t i = row.size(); i < headerCells_.size(); ++i)
        {
          result << "<td></td>";
        }
        result << "</tr>";
      }
      result << "</tbody>";
    }

    result << "</table>";
  }

private:
  bool isFinished_;
  uint32_t lineIndex_;
  std::vector<std::string> headerCells_;
  std::vector<std::string> alignments_;
  std::vector<std::vector<std::string>> bodyRows_;

  void finish()
  {
    std::string emptyLine = "";
    this->parseBlock(emptyLine);
    this->isFinished_ = true;
  }

  static std::string trimWhitespace(const std::string& str)
  {
    auto start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    auto end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
  }

  static std::vector<std::string> splitRow(const std::string& row)
  {
    std::vector<std::string> cells;
    // Strip leading and trailing `|`
    std::string inner = row;
    if (!inner.empty() && inner.front() == '|') inner.erase(0, 1);
    if (!inner.empty() && inner.back() == '|') inner.pop_back();

    std::string cell;
    for (size_t i = 0; i < inner.size(); ++i)
    {
      if (inner[i] == '\\' && i + 1 < inner.size() && inner[i + 1] == '|')
      {
        cell += '|';
        ++i;
      }
      else if (inner[i] == '|')
      {
        cells.push_back(trimWhitespace(cell));
        cell.clear();
      }
      else
      {
        cell += inner[i];
      }
    }
    cells.push_back(trimWhitespace(cell));
    return cells;
  }
}; // class GfmTableParser

// -----------------------------------------------------------------------------

} // namespace maddy

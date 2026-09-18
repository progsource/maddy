/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <cstddef>
#include <functional>
#include <string>
#include <utility>
#include <vector>

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * FindProtectedSpans
 *
 * Finds non-overlapping [start, end) ranges of a line that inline
 * delimiter parsers (strong, emphasized, strikethrough, italic) must not
 * alter: a `<code>...</code>` HTML span, or a code span delimited by a
 * run of N backtick characters and a later run of exactly N backticks
 * (per CommonMark, a run with no matching close is ordinary text, and
 * scanning resumes right after it).
 *
 * @method
 * @param {const std::string&} line
 * @return {std::vector<std::pair<std::size_t, std::size_t>>}
 */
inline std::vector<std::pair<std::size_t, std::size_t>> FindProtectedSpans(
  const std::string& line
)
{
  static const std::string openCodeTag = "<code>";
  static const std::string closeCodeTag = "</code>";

  std::vector<std::pair<std::size_t, std::size_t>> spans;
  std::size_t i = 0;

  while (i < line.size())
  {
    if (line.compare(i, openCodeTag.size(), openCodeTag) == 0)
    {
      std::size_t closeStart = line.find(closeCodeTag, i + openCodeTag.size());
      if (closeStart != std::string::npos)
      {
        std::size_t end = closeStart + closeCodeTag.size();
        spans.emplace_back(i, end);
        i = end;
        continue;
      }
    }

    if (line[i] == '`')
    {
      std::size_t runStart = i;
      while (i < line.size() && line[i] == '`')
      {
        ++i;
      }
      std::size_t runLength = i - runStart;

      std::size_t searchPos = i;
      while (searchPos < line.size())
      {
        std::size_t closeStart = line.find('`', searchPos);
        if (closeStart == std::string::npos)
        {
          break;
        }

        std::size_t closeEnd = closeStart;
        while (closeEnd < line.size() && line[closeEnd] == '`')
        {
          ++closeEnd;
        }

        if (closeEnd - closeStart == runLength)
        {
          spans.emplace_back(runStart, closeEnd);
          i = closeEnd;
          break;
        }

        searchPos = closeEnd;
      }

      continue;
    }

    ++i;
  }

  return spans;
}

/**
 * ApplyOutsideProtectedSpans
 *
 * Runs `transform` on each stretch of `line` that falls outside its
 * protected spans (see FindProtectedSpans), leaving the spans themselves
 * untouched, then writes the reassembled result back into `line`.
 *
 * @method
 * @param {std::string&} line
 * @param {const std::function<void(std::string&)>&} transform
 * @return {void}
 */
inline void ApplyOutsideProtectedSpans(
  std::string& line, const std::function<void(std::string&)>& transform
)
{
  std::string result;
  std::size_t pos = 0;

  for (const auto& span : FindProtectedSpans(line))
  {
    std::string segment = line.substr(pos, span.first - pos);
    transform(segment);
    result += segment;
    result += line.substr(span.first, span.second - span.first);
    pos = span.second;
  }

  std::string tail = line.substr(pos);
  transform(tail);
  result += tail;

  line = result;
}

// -----------------------------------------------------------------------------

} // namespace maddy

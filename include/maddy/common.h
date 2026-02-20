#pragma once

#include <string>

namespace maddy {

namespace common {

inline std::string escapeHTML(const std::string& input)
{
  std::string result;

  for (char c : input)
  {
    switch (c)
    {
      case '&':
        result += "&amp;";
        break;
      case '<':
        result += "&lt;";
        break;
      case '>':
        result += "&gt;";
        break;
      case '"':
        result += "&quot;";
        break;
      case '\'':
        result += "&apos;";
        break;
      default:
        result += c;
    }
  }

  return result;
}

} // namespace common

} // namespace maddy

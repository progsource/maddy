#include "maddy/parser.h"
#include <string>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string payload(reinterpret_cast<const char *>(data), size);
  auto parser = std::make_shared<maddy::Parser>();
  std::stringstream markdown(payload);
  parser->Parse(markdown);

  return 0;
}
#pragma once

#include <string>

namespace MORL {
  enum class TerminalColor {
    HeroText = 1,
    Default,
    Important,
    Warning,
    Error,
  };

  constexpr unsigned ReadStringBufferSize = 200;

  void SetColor(TerminalColor color);
  std::string ReadString();
}

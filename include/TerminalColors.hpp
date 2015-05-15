#pragma once

namespace MORL {
  enum class TerminalColor {
    HeroText = 1,
    Default,
    Important,
    Warning,
    Error,
  };

  void SetColor(TerminalColor color);
}
#include "TerminalColors.hpp"

#include <curses.h>

namespace MORL {
  void SetColor(TerminalColor color) {
    attron(COLOR_PAIR((int)color));
  }
}
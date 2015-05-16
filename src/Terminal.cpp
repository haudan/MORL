#include "Terminal.hpp"

#include <curses.h>

#include <memory>

namespace MORL {
  void SetColor(TerminalColor color) {
    attron(COLOR_PAIR((int)color));
  }

  std::string ReadString() {
    nodelay(stdscr, false);
    echo();

    std::unique_ptr<char[]> buf(new char[ReadStringBufferSize]);
    getnstr(buf.get(), ReadStringBufferSize - 1);
    buf[ReadStringBufferSize - 1] = 0;
    std::string str{buf.get()};

    noecho();
    nodelay(stdscr, true);
    return str;
  }
}

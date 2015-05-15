#include "Screen.hpp"
#include "TerminalColors.hpp"

#include <curses.h>

namespace MORL {
  Screen::Screen(OwnedScreenState && initialState) {
    InitCurse();
    mScreenStates.push(std::move(initialState));
  }

  Screen::Screen(Screen && other)
  : mScreenStates(std::move(other.mScreenStates)),
    mScreenWidth(other.mScreenWidth),
    mScreenHeight(other.mScreenHeight)
  {}

  Screen::~Screen() {
    FinaCurse();
  }

  void Screen::InitCurse() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    getmaxyx(stdscr, mScreenHeight, mScreenWidth);

    // Init colors
    start_color();
    init_pair((int)TerminalColor::HeroText, COLOR_RED, COLOR_BLACK);
    init_pair((int)TerminalColor::Default, COLOR_WHITE, COLOR_BLACK);
    init_pair((int)TerminalColor::Important, COLOR_RED, COLOR_CYAN);
    init_pair((int)TerminalColor::Warning, COLOR_YELLOW, COLOR_BLACK);
    init_pair((int)TerminalColor::Error, COLOR_BLACK, COLOR_RED);
  }

  void Screen::FinaCurse() {
    endwin();
  }

  void Screen::GotoScreen(OwnedScreenState && screenState) {
    mScreenStates.push(std::move(screenState));
  }

  bool Screen::GoBack() {
    if(mScreenStates.size() <= 1) {
      return false;
    }

    mScreenStates.pop();
    CurrentScreenState().SetDirty();
    return true;
  }

  void Screen::Update() {
    if(is_termresized()) {
      getmaxyx(stdscr, mScreenHeight, mScreenWidth);
    }

    auto &curScreenState = CurrentScreenState();
    curScreenState.Update();
  }

  void Screen::Refresh() {
    auto &curScreenState = CurrentScreenState();

    if(curScreenState.IsDirty()) {
      curScreenState.Draw();
      refresh();
      curScreenState.SetClean();
    }
  }
}
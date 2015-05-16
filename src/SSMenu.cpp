#include <curses.h>

#include "SSMenu.hpp"

#include "Game.hpp"
#include "Terminal.hpp"
#include "SSOptions.hpp"

namespace MORL {
  SSMenu::SSMenu(Game &game)
  : ScreenState(game)
  {}

  void SSMenu::Update() {
    auto &keyboard = mGame.Keyboard();
    if(keyboard.IsKeyDown('q')) {
      mGame.Quit();
    }

    if(keyboard.IsKeyDown('e')) {
      mGame.Screen().GotoScreen(MakeUnique<SSOptions>(mGame));
    }

    if(keyboard.IsKeyDown('w')) {
      mRenderIpPrompt = true;
      RequireRedraw();
    }
  }

  void SSMenu::Draw() {
    clear();
    SetColor(TerminalColor::HeroText);
    printw("Hey there, welcome to the main menu!\n");
    SetColor(TerminalColor::Default);
    printw("Press 'q' to quit, 'e' for the options menu\n");
    printw("Press 'w' to connect to a server");

    if(mRenderIpPrompt) {
      mvprintw(mGame.Screen().Height() - 1, 0, "IP: ");
      SetColor(TerminalColor::Warning);
      auto input = ReadString();
      SetColor(TerminalColor::Default);
      mvprintw(mGame.Screen().Height() - 1, 0, "You entered: %s", input.c_str());
      mRenderIpPrompt = false;
    }
  }
}

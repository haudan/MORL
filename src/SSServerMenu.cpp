#include <curses.h>
#include <Terminal.hpp>

#include "SSServerMenu.hpp"
#include "Network/StateGameplay.hpp"

#include "Game.hpp"

namespace MORL {
  SSServerMenu::SSServerMenu(Game &game)
    : ScreenState(game) {
    Network::StateGameplay gameplay{mGame.Session()};
    gameplay.SetNumClientsChangeCallback([&] (auto &clients) {
      this->mClients = &clients;
      this->RequireRedraw();
    });
    mGame.Session().PushState(std::move(gameplay));
  }

  void SSServerMenu::Update() {
    if(mGame.Keyboard().IsKeyDown('q')) {
      mGame.Quit();
    }
  }

  void SSServerMenu::Draw() {
    SetColor(TerminalColor::Default);
    clear();
    printw("Server running on port ");
    SetColor(TerminalColor::HeroText);
    printw("%d\n\n", mGame.Session().Socket().LocalPort());
    SetColor(TerminalColor::Default);
    printw("Connected clients:\n");

    if(mClients) {
      SetColor(TerminalColor::Important);

      for(auto const &client : *mClients) {
        printw("%s\n", static_cast<std::string>(client).c_str());
      }
    }
    else {
      SetColor(TerminalColor::Warning);
      printw("No clients connected!");
    }
  }
}
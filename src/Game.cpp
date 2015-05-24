#include <curses.h>

#include "Game.hpp"

class FooEntity : public MORL::Gameplay::WorldEntity {
public:
  void Update() override {}
  void Draw() const override {
    mvprintw(0, 0, "Hi, I'm a foo entity!");
  }
};

namespace MORL {
  Game::Game() {
    mGameplayGame.World().AddEntity(FooEntity{});

    // Brainstorming on how to handle the moving camera
  }

  void Game::Update() {
    mKeyboard.Update();
    mSession.Update();
    mScreen.Update();
  }

  void Game::Draw() {
    mScreen.Refresh();
  }
}
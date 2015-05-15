#pragma once

#include "Keyboard.hpp"
#include "ScreenState.hpp"
#include "SSMenu.hpp"
#include "Screen.hpp"
#include "Util.hpp"

namespace MORL {
  class Game {
  public:
    Game() = default;
    Game(Game const &other) = delete;
    ~Game() = default;

    void Update();
    void Draw();

    inline bool Running() const {
      return mRunning;
    }

    inline void Quit() {
      mRunning = false;
    }

    inline MORL::Keyboard const &Keyboard() const {
      return mKeyboard;
    }

    inline MORL::Screen &Screen() {
      return mScreen;
    }
  private:
    bool mRunning = true;
    MORL::Keyboard mKeyboard;
    MORL::Screen mScreen{MakeUnique<MORL::SSMenu>(*this)};
  };
}
#include "Keyboard.hpp"

#include <curses.h>

namespace MORL {
  bool Keyboard::IsKeyDown(int key) const {
    auto iter = mKeyStates.find(key);
    if(iter == mKeyStates.end()) {
      return false;
    }

    return iter->second;
  }

  void Keyboard::Update() {
    for(auto &pair : mKeyStates) {
      pair.second = false;
    }

    auto key = getch();
    if(key != ERR) {
      mKeyStates[key] = true;
    }
  }
}
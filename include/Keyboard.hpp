#pragma once

#include <unordered_map>

namespace MORL {
  class Keyboard {
  public:
    bool IsKeyDown(int key) const;

    inline bool IsKeyUp(int key) const {
      return !IsKeyDown(key);
    }

    /**
     * Fetch for keyboard input and update the internal key states
     */
    void Update();

  private:
    std::unordered_map<int, bool> mKeyStates;
  };
}
#pragma once

#include "Gameplay/World.hpp"
#include "FrameBuffer.hpp"

namespace MORL {
  namespace Gameplay {
    class Game {
    public:
      static constexpr int GameScreenWidth = 60;
      static constexpr int GameScreenHeight = 25;

      Game() = default;
      Game(Game const &other) = default;
      ~Game() = default;

      inline Gameplay::World const &World() const {
        return mWorld;
      }

      inline Gameplay::World &World() {
        return mWorld;
      }

      void Update();
      void Draw();
      void DrawToTerminal(int maxWidth, int maxHeight);
    private:
      Gameplay::World mWorld;
      FrameBuffer mFrameBuffer{GameScreenWidth, GameScreenHeight};
    };
  }
}
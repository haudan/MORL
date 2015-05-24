#pragma once

#include "Gameplay/World.hpp"

namespace MORL {
  namespace Gameplay {
    class Game {
    public:
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
    private:
      Gameplay::World mWorld;
    };
  }
}
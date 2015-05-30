#pragma once

#include "Gameplay/WorldEntity.hpp"

namespace MORL {
  namespace Gameplay {
    class Player;

    class PlayerEntity : public WorldEntity {
    public:
      PlayerEntity() = delete;
      PlayerEntity(PlayerEntity const &other) = default;
      PlayerEntity(Player &player);
      ~PlayerEntity();

      void Update() override;
      void Draw(FrameBuffer &frameBuffer) const override;
    private:
      // The actual player this entity responds to
      Player &mPlayer;
    };
  }
}
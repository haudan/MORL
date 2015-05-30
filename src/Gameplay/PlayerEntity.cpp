#include "Gameplay/PlayerEntity.hpp"

#include "FrameBuffer.hpp"
#include "Gameplay/Player.hpp"

namespace MORL {
  namespace Gameplay {
    PlayerEntity::PlayerEntity(Player &player)
      : mPlayer(player) {
      player.PlayerEntity(this);
    }

    PlayerEntity::~PlayerEntity() {
      //mPlayer.PlayerEntity(nullptr);
    }

    void PlayerEntity::Update() {
      mPosition = mPlayer.Position();
    }

    void PlayerEntity::Draw(FrameBuffer &frameBuffer) const {
      frameBuffer.SetCharAt(mPosition.X(), mPosition.Y(), '@');
      frameBuffer.Text(mPosition.X(), mPosition.Y() + 1, mPlayer.Name());
    }
  }
}
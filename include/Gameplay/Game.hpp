#pragma once

#include "Gameplay/World.hpp"
#include "Gameplay/Player.hpp"
#include "FrameBuffer.hpp"

#include "Network/Packets/PlayerUpdatePacket.hpp"

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

      #ifndef MORL_SERVER_SIDE
      inline Player const *LocalPlayer() const {
        return mLocalPlayer.get();
      }

      inline bool DoesLocalPlayerExist() {
        return mLocalPlayer != nullptr;
      }

      void LocalPlayerUpdate(Network::PlayerUpdatePacket &updatePacket);
      #endif

      void Update();
      void Draw();
      void DrawToTerminal(int maxWidth, int maxHeight);
    private:
      Gameplay::World mWorld;
      FrameBuffer mFrameBuffer{GameScreenWidth, GameScreenHeight};
      #ifdef MORL_SERVER_SIDE
      #else
      // Self
      std::unique_ptr<Player> mLocalPlayer;
      #endif
    };
  }
}
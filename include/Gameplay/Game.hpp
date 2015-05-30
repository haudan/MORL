#pragma once

#include "Gameplay/World.hpp"
#include "Gameplay/Player.hpp"
#include "Gameplay/PlayerEntity.hpp"
#include "FrameBuffer.hpp"
#include "IPEndpoint.hpp"

#include "Network/Packets/PlayerUpdatePacket.hpp"
#include "Network/Packets/EntityEventPacket.hpp"

namespace MORL {
  namespace Gameplay {
    class Game {
    public:
      static constexpr int GameScreenWidth = 60;
      static constexpr int GameScreenHeight = 25;

      #ifdef MORL_SERVER_SIDE
      using ConnectedPlayersContainer = std::unordered_map<IPEndpoint, Player>;
      #else
      using ConnectedPlayersContainer = std::unordered_map<size_t, Player>;
      #endif

      Game() = default;
      Game(Game const &other) = default;
      ~Game() = default;

      inline Gameplay::World const &World() const {
        return mWorld;
      }

      inline Gameplay::World &World() {
        return mWorld;
      }

      #ifdef MORL_SERVER_SIDE
      inline ConnectedPlayersContainer const &ConnectedPlayers() const {
        return mConnectedPlayers;
      }

      inline void AddConnectedPlayer(IPEndpoint const &addr, Player const &player) {
        mConnectedPlayers.emplace(addr, player);
      }

      inline void RemoveConnectedPlayer(IPEndpoint const &addr) {
        auto iter = mConnectedPlayers.find(addr);
        if(iter != mConnectedPlayers.end()) {
          mConnectedPlayers.erase(iter);
        }
      }

      inline Player* GetConnectedPlayer(IPEndpoint const &addr) {
        auto iter = mConnectedPlayers.find(addr);
        if(iter == mConnectedPlayers.end()) {
          return nullptr;
        }

        return &iter->second;
      }
      #else
      inline Player *LocalPlayer() {
        return mLocalPlayer.get();
      }

      /*inline bool DoesLocalPlayerExist() const {
        return mLocalPlayer != nullptr;
      }*/

      void LocalPlayerUpdate(Network::PlayerUpdatePacket const &updatePacket);

      void LocalEntityEvent(Network::EntityEventPacket const &eventPacket);
      #endif

      void Update();
      void Draw();
      void DrawToTerminal(int maxWidth, int maxHeight);
    private:
      Gameplay::World mWorld;
      FrameBuffer mFrameBuffer{GameScreenWidth, GameScreenHeight};
      ConnectedPlayersContainer mConnectedPlayers;
      #ifdef MORL_SERVER_SIDE
      #else
      // Local player
      //Player *mLocalPlayer = nullptr;
      std::unique_ptr<Player> mLocalPlayer;
      PlayerEntity *mLocalPlayerEntity = nullptr;
      //std::unique_ptr<PlayerEntity> mLocalPlayerEntity;
      #endif
    };
  }
}
#pragma once

#include "Vec2.hpp"
#include "Network/Packets/PlayerMovementPacket.hpp"

#include <string>

namespace MORL {
  namespace Gameplay {
    class Player {
    public:
      Player() = delete;
      Player(Player const &other) = default;
      Player(std::string const &name);
      Player(std::string const &name, Vec2i const &position);
      ~Player() = default;

      inline Vec2i const &Position() const {
        return mPosition;
      }

      inline void Position(Vec2i const &pos) {
        mPosition = pos;
      }

      inline std::string const &Name() const {
        return mName;
      }

      inline void Name(std::string const &name) {
        mName = name;
      }

      /**
       * Make the player move in the desired direction
       *
       * @param Network::PlayerMovementPacket::Direction dir The direction to move
       */
      void Move(Network::PlayerMovementPacket::Direction dir);
    private:
      // Player's position in the world
      Vec2i mPosition;
      // Name
      std::string mName;
    };
  }
}
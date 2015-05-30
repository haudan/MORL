#include "Gameplay/Player.hpp"

namespace MORL {
  namespace Gameplay {
    Player::Player(std::string const &name)
      : mName(name)
    {}

    Player::Player(std::string const &name, Vec2i const &position)
      : mPosition(position),
        mName(name)
    {}

    void Player::Move(Network::PlayerMovementPacket::Direction dir) {
      // TODO: Obviously do collision detection & stuff here
      using Dir = Network::PlayerMovementPacket::Direction;
      switch(dir) {
        case Dir::Up:
          mPosition.Y(mPosition.Y() - 1);
          break;
        case Dir::Down:
          mPosition.Y(mPosition.Y() + 1);
          break;
        case Dir::Left:
          mPosition.X(mPosition.X() - 1);
          break;
        case Dir::Right:
          mPosition.X(mPosition.X() + 1);
          break;
      }
    }
  }
}
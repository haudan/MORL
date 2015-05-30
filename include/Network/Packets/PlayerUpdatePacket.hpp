#pragma once

namespace MORL {
  namespace Network {
    constexpr size_t PlayerUpdateMaxNameLength = 40;

    struct PlayerUpdatePacket {
      int newX;
      int newY;
      char name[PlayerUpdateMaxNameLength + 1];
    };
  }
}
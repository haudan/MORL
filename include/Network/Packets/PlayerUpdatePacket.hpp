#pragma once

#include <cstdint>

namespace MORL {
  namespace Network {
    constexpr size_t PlayerUpdateMaxNameLength = 40;

    struct PlayerUpdatePacket {
      uint8_t isMe; // Tells the recipient if this is his player's data
      uint32_t playerId;
      int32_t newX;
      int32_t newY;
      char name[PlayerUpdateMaxNameLength + 1];
    };
  }
}
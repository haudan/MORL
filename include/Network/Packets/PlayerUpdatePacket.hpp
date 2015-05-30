#pragma once

#include <cstdint>

namespace MORL {
  namespace Network {
    constexpr size_t PlayerUpdateMaxNameLength = 40;

    struct PlayerUpdatePacket {
      int32_t newX;
      int32_t newY;
      char name[PlayerUpdateMaxNameLength + 1];
    };
  }
}
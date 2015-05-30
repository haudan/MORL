#pragma once

namespace MORL {
  namespace Network {
    struct DisconnectPacket {
      uint32_t playerId; // Only used by server
    };
  }
}
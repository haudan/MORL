#pragma once

namespace MORL {
  namespace Network {
    struct ConnectPacket {
      bool serverAknowledged; // Only used by server
    };
  }
}
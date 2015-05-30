#pragma once

#include "Gameplay/WorldEntityType.hpp"

namespace MORL {
  namespace Network {
    struct EntityEventData {
      uint32_t entityId;
      Gameplay::WorldEntityType entityType;

      uint32_t entityX;
      uint32_t entityY;
    };

    struct EntityEventPacket {
      enum Type {
        NewEntity,
        UpdateEntity,
        DeleteEntity
      } type;

      EntityEventData data;
    };
  }
}
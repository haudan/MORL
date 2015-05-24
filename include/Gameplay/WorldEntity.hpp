#pragma once

#include "Vec2.hpp"

namespace MORL {
  class FrameBuffer;

  namespace Gameplay {
    class WorldEntity {
    public:
      /**
       * Update the entity
       */
      virtual void Update() = 0;

      /**
       * Draw the entity
       */
      virtual void Draw(FrameBuffer &frameBuffer) const = 0;

      inline Vec2i const &Position() const {
        return mPosition;
      }

      inline void Position(Vec2i const &pos) {
        mPosition = pos;
      }
    protected:
      Vec2i mPosition;
    };
  }
}
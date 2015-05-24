#pragma once

#include "Gameplay/WorldEntity.hpp"
#include "Util.hpp"

#include <vector>
#include <algorithm>

namespace MORL {
  class FrameBuffer;

  namespace Gameplay {
    class World {
    public:
      using OwnedWorldEntity = std::unique_ptr<WorldEntity>;
      using WorldEntityContainer = std::vector<OwnedWorldEntity>;

      World() = default;
      World(World const &other) = default;
      World(World && other);
      ~World() = default;

      /**
       * Add a new entity to the world
       * @return WorldEntity* A pointer to the new entity
       */
      template <typename E>
      inline WorldEntity *AddEntity(E const &entity) {
        auto owned = MakeUnique<E>(entity);
        mEntities.push_back(std::move(owned));
        return owned.get();
      }

      /**
       * Remove the entity from the world
       * @param WorldEntity* entity The world to remove
       */
      void RemoveEntity(WorldEntity *entity);

      /**
       * Update the world
       */
      void Update();

      /**
       * Draw the world to the framebuffer
       */
      void Draw(FrameBuffer &frameBuffer) const;
    private:
      WorldEntityContainer mEntities;
    };
  }
}